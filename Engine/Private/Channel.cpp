#include "Channel.h"

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

void CChannel::Update_TransformationMatrix(vector<CBone*>& Bones, _float fCurrentTrackPosition)
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
		if (fCurrentTrackPosition >= m_KeyFrames[(_uInt)m_iCurrentKeyFrameIndex + 1].fTrackPosition)
			m_iCurrentKeyFrameIndex++;

		_float3		vSourScale{}, vDestScale{};
		_float4		vSourRotation{}, vDestRotation{};
		_float3		vSourTranslation{}, vDestTranslation{};

		vSourScale = m_KeyFrames[m_iCurrentKeyFrameIndex].vScale;
		vDestScale = m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vScale;

		vSourRotation = m_KeyFrames[m_iCurrentKeyFrameIndex].vRotation;
		vDestRotation = m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vRotation;

		vSourTranslation = m_KeyFrames[m_iCurrentKeyFrameIndex].vTranslation;
		vDestTranslation = m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vTranslation;

		// 수식 
		// 현재 프레임기준 진행된 길이 : 현재 트랙 위치 - 현재 프레임기준 왼쪽 Frame노드
		// 현재 프레임 과 다음 프레임 사이거리 : 현재 프레임기준 오른쪽 Frame노드 - 현재 프레임기준 왼쪽 Frame노드
		// 진행된 길이 / 프레임 사이거리 = 진행된 비율값
		// 즉 현재 진행 위치로 부터 다음위치까지의 진행 비율이다.
		// 이걸이용해 선형 보간
		_float		fRatio = (fCurrentTrackPosition - m_KeyFrames[m_iCurrentKeyFrameIndex].fTrackPosition) /
							 (m_KeyFrames[m_iCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[m_iCurrentKeyFrameIndex].fTrackPosition);

		vScale = XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), fRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), fRatio);
		vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&vSourTranslation), XMLoadFloat3(&vDestTranslation), fRatio), 1.f);
	}
	
	_matrix		BoneTransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	Bones[m_iBoneIndex]->SetBoneTransformMatrix(BoneTransformationMatrix);
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

void CChannel::Free()
{
    __super::Free();
}
