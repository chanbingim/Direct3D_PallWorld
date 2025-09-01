#include "Channel.h"

#include "PaseDataHeader.h"
#include "Model.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const CModel* pModel, const aiNodeAnim* pAIChannel)
{
    strcpy_s(m_szName, pAIChannel->mNodeName.data);
    m_iBoneIndex = pModel->GetBoneIndex(m_szName);
    m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, max(pAIChannel->mNumPositionKeys, pAIChannel->mNumRotationKeys));

	KEYFRAME		KeyFrame{};
    for (size_t i = 0; i < m_iNumKeyFrames; i++)
    {
		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&KeyFrame.vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTrackPosition = (_float)pAIChannel->mScalingKeys[i].mTime;
		}

		if (i < pAIChannel->mNumRotationKeys)
		{
			KeyFrame.vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			KeyFrame.vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			KeyFrame.vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			KeyFrame.vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;

			KeyFrame.fTrackPosition = (_float)pAIChannel->mRotationKeys[i].mTime;
		}

		if (i < pAIChannel->mNumPositionKeys)
		{
			memcpy(&KeyFrame.vTranslation, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTrackPosition = (_float)pAIChannel->mPositionKeys[i].mTime;
		}
		m_KeyFrames.push_back(KeyFrame);
    }
    return S_OK;
}

HRESULT CChannel::Initialize(void* pArg)
{
	SAVE_CHANNEL_DESC* pChannel = static_cast<SAVE_CHANNEL_DESC *>(pArg);

	strcpy_s(m_szName, pChannel->szName);
	m_iBoneIndex = pChannel->iBoneIndex;
	m_iNumKeyFrames = pChannel->iNumKeyFrames;
	m_KeyFrames = pChannel->KeyFrames;

	return S_OK;
}

void CChannel::Update_TransformationMatrix(vector<CBone*>& Bones, _float fCurrentTrackPosition, _uInt* iKeyIndex)
{
	KEYFRAME	LastFrame = m_KeyFrames.back();
	_vector		vScale{}, vRotation{}, vTranslation{};

	if (LastFrame.fTrackPosition <= fCurrentTrackPosition)
	{
		vScale = XMLoadFloat3(&LastFrame.vScale);
		vRotation = XMLoadFloat4(&LastFrame.vRotation);
		vTranslation = XMLoadFloat3(&LastFrame.vTranslation);
	}
	else
	{
		if (0.f == fCurrentTrackPosition)
			(*iKeyIndex) = 0;

		while (fCurrentTrackPosition >= m_KeyFrames[(_uInt)(*iKeyIndex) + 1].fTrackPosition)
				(*iKeyIndex)++;

		_float3		vSourScale{}, vDestScale{};
		_float4		vSourRotation{}, vDestRotation{};
		_float3		vSourTranslation{}, vDestTranslation{};

		vSourScale = m_KeyFrames[(*iKeyIndex)].vScale;
		vDestScale = m_KeyFrames[(*iKeyIndex) + 1].vScale;

		vSourRotation = m_KeyFrames[(*iKeyIndex)].vRotation;
		vDestRotation = m_KeyFrames[(*iKeyIndex) + 1].vRotation;

		vSourTranslation = m_KeyFrames[(*iKeyIndex)].vTranslation;
		vDestTranslation = m_KeyFrames[(*iKeyIndex) + 1].vTranslation;

		// 수식 
		// 현재 프레임기준 진행된 길이 : 현재 트랙 위치 - 현재 프레임기준 왼쪽 Frame노드
		// 현재 프레임 과 다음 프레임 사이거리 : 현재 프레임기준 오른쪽 Frame노드 - 현재 프레임기준 왼쪽 Frame노드
		// 진행된 길이 / 프레임 사이거리 = 진행된 비율값
		// 즉 현재 진행 위치로 부터 다음위치까지의 진행 비율이다.
		// 이걸이용해 선형 보간
		_float		fRatio = (fCurrentTrackPosition - m_KeyFrames[(*iKeyIndex)].fTrackPosition) /
							 (m_KeyFrames[(*iKeyIndex) + 1].fTrackPosition - m_KeyFrames[(*iKeyIndex)].fTrackPosition);

		vScale = XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), fRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), fRatio);
		vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&vSourTranslation), XMLoadFloat3(&vDestTranslation), fRatio), 1.f);
	}
	
	_matrix		BoneTransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	Bones[m_iBoneIndex]->SetBoneTransformMatrix(BoneTransformationMatrix);
}

void CChannel::Update_TransformationMatrix(vector<CBone*>& Bones, _float fRatio)
{
	if (!m_bIsBoneLerp)
	{
		XMStoreFloat4x4(&m_PreBoneMatrix, Bones[m_iBoneIndex]->GetBoneTransformMatrix());
		m_bIsBoneLerp = true;
	}

	_vector		vScale{}, vRotation{}, vTranslation{};
	_vector		vSourScale{}, vSourRotation{}, vSourTranslation{};

	_float3		vDestScale{}, vDestTranslation{};
	_float4		vDestRotation{};

	_matrix PreBoneMatrix = XMLoadFloat4x4(&m_PreBoneMatrix);
	XMMatrixDecompose(&vSourScale, &vSourRotation, &vSourTranslation, PreBoneMatrix);

	vDestScale = m_KeyFrames[0].vScale;
	vDestRotation = m_KeyFrames[0].vRotation;
	vDestTranslation = m_KeyFrames[0].vTranslation;

	vScale = XMVectorLerp(vSourScale, XMLoadFloat3(&vDestScale), fRatio);
	vRotation = XMQuaternionSlerp(vSourRotation, XMLoadFloat4(&vDestRotation), fRatio);
	vTranslation = XMVectorSetW(XMVectorLerp(vSourTranslation, XMLoadFloat3(&vDestTranslation), fRatio), 1.f);

	_matrix		BoneTransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	Bones[m_iBoneIndex]->SetBoneTransformMatrix(BoneTransformationMatrix);

	if(1.0f <= fRatio)
		m_bIsBoneLerp = false;
}

void CChannel::Export(void* pChannelDesc)
{
	SAVE_CHANNEL_DESC* ChannelDesc = static_cast<SAVE_CHANNEL_DESC*>(pChannelDesc);
	strcpy_s(ChannelDesc->szName, m_szName);
	ChannelDesc->iBoneIndex = m_iBoneIndex;
	ChannelDesc->iNumKeyFrames = m_iNumKeyFrames;
	ChannelDesc->KeyFrames = m_KeyFrames;
}

_float CChannel::GetPreKeyFrameTrackPos(_uInt iIndex)
{
	return m_KeyFrames[iIndex].fTrackPosition;
}

CChannel* CChannel::Create(const CModel* pModel, const aiNodeAnim* pAIChannel)
{
    CChannel* pChannel = new CChannel();
    if (FAILED(pChannel->Initialize(pModel, pAIChannel)))
    {
        Safe_Release(pChannel);
        MSG_BOX("CREATE FAIL : ANIM CHANNEL");
    }
    return pChannel;
}

CChannel* CChannel::Create(void* pArg)
{
	CChannel* pChannel = new CChannel();
	if (FAILED(pChannel->Initialize(pArg)))
	{
		Safe_Release(pChannel);
		MSG_BOX("CREATE FAIL : ANIM CHANNEL");
	}
	return pChannel;
}

void CChannel::Free()
{
    __super::Free();
}
