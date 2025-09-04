#include "Animation.h"

#include "PaseDataHeader.h"
#include "Channel.h"
#include "Model.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& rhs) :
	m_fCurrentTrackPosition(rhs.m_fCurrentTrackPosition),
	m_fLength(rhs.m_fLength),
	m_fTickPerSecond(rhs.m_fTickPerSecond),
	m_iNumChannels(rhs.m_iNumChannels),
	m_Channels(rhs.m_Channels),
	m_iChannelIndex(rhs.m_iChannelIndex)
{
	strcpy_s(m_szAnimName, rhs.m_szAnimName);

	for (auto& iter : m_Channels)
		Safe_AddRef(iter);
}

HRESULT CAnimation::Initialize(const CModel* pModel, const aiAnimation* pAIAnimation, _bool bIsLoop)
{
	strcpy_s(m_szAnimName, MAX_PATH, strchr(pAIAnimation->mName.data, '|') + 1);

	m_fLength = (_float)pAIAnimation->mDuration;
	m_fTickPerSecond = (_float)pAIAnimation->mTicksPerSecond;
	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uInt i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(pModel, pAIAnimation->mChannels[i]);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
		m_iChannelIndex.push_back(0);
	}
	return S_OK;
}

HRESULT CAnimation::Initialize(void* pArg)
{
	SAVE_ANIMATION_DESC* AnimDesc = static_cast<SAVE_ANIMATION_DESC*>(pArg);
	strcpy_s(m_szAnimName, AnimDesc->szAnimName);

	m_fLength = AnimDesc->fLength;
	m_fTickPerSecond = AnimDesc->fTickPerSecond;
	m_iNumChannels = AnimDesc->iNumChannels;

	for (auto ChannelDesc : AnimDesc->Channels)
	{
		CChannel* pChannel = CChannel::Create(&ChannelDesc);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
		m_iChannelIndex.push_back(0);
	}
	return S_OK;
}

_bool CAnimation::UpdateTransformationMatrices(vector<CBone*>& Bones, _float fTimeDelta, _int2 UpdateBoneIdx, _bool bIsLoop)
{
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;
	if (m_fCurrentTrackPosition >= m_fLength)
	{
		m_fCurrentTrackPosition = 0.f;
		if (!bIsLoop)
		{
			fill(m_iChannelIndex.begin(), m_iChannelIndex.end(), 0);
			return true;
		}
	}

	_uInt iIndex = {};
	for (auto& pChannel : m_Channels)
	{
		_Int ChannelBoneIdx = pChannel->GetBoneIndex();
		if (UpdateBoneIdx.x <= ChannelBoneIdx && UpdateBoneIdx.y >= ChannelBoneIdx)
			pChannel->Update_TransformationMatrix(Bones, m_fCurrentTrackPosition, &m_iChannelIndex[iIndex++]);
	}
	return false;
}

_bool CAnimation::UpdateTransformationMatrices(vector<CBone*>& Bones, _float fTimeDelta, _int2 UpdateBoneIdx, _float2* LastAnimTrackPos, _float fLength, _float LerpSpeed)
{
	LastAnimTrackPos->x += LerpSpeed * fTimeDelta;
	_float fRatio = LastAnimTrackPos->x / 0.2f; //(LastAnimTrackPos->x - LastAnimTrackPos->y) / 0.2f;
	fRatio = Clamp<_float>(fRatio, 0.f, 1.f);
	for (auto& pChannel : m_Channels)
	{
		_Int ChannelBoneIdx = pChannel->GetBoneIndex();
		if (UpdateBoneIdx.x <= ChannelBoneIdx && UpdateBoneIdx.y >= ChannelBoneIdx)
			pChannel->Update_TransformationMatrix(Bones, fRatio);
	}

	if (1.0f <= fRatio)
		return true;

	return false;
}

_bool CAnimation::CompareAnimName(const char* szName)
{
	return !strcmp(szName, m_szAnimName);
}

_float2 CAnimation::GetPreFrameKey()
{
	_uInt iIndex = 0;
	for (auto& iter : m_iChannelIndex)
		iIndex = max(iter, iIndex);

	_float2 ReturnData = { 0 , m_Channels[iIndex]->GetPreKeyFrameTrackPos(m_iChannelIndex[iIndex]) };
	m_fCurrentTrackPosition = 0;
	fill(m_iChannelIndex.begin(), m_iChannelIndex.end(), 0);
	return ReturnData;
}

void CAnimation::Export(void* pAnimationDesc)
{
	SAVE_ANIMATION_DESC* AnimDesc = static_cast<SAVE_ANIMATION_DESC *>(pAnimationDesc);

	strcpy_s(AnimDesc->szAnimName, m_szAnimName);
	AnimDesc->fLength = m_fLength;
	AnimDesc->fTickPerSecond = m_fTickPerSecond;
	AnimDesc->iNumChannels = m_iNumChannels;
	
	AnimDesc->Channels.reserve(m_iNumChannels);
	for (_uInt i = 0; i < m_iNumChannels; ++i)
	{
		SAVE_CHANNEL_DESC ChannelDesc = {};
		m_Channels[i]->Export(&ChannelDesc);

		AnimDesc->Channels.push_back(ChannelDesc);
	}
}

void CAnimation::GetUseBoneIndex(vector<_bool>& BoneList)
{
	for (auto& pChannel : m_Channels)
		BoneList[pChannel->GetBoneIndex()] = true;
}

CAnimation* CAnimation::Create(const CModel* pModel, const aiAnimation* pAIAnimation, _bool bIsLoop)
{
	CAnimation* pAnim = new CAnimation();
	if (FAILED(pAnim->Initialize(pModel, pAIAnimation, bIsLoop)))
	{
		Safe_Release(pAnim);
		MSG_BOX("CREATE FAIL : MODEL ANIMATION ");
	}
	return pAnim;
}

CAnimation* CAnimation::Create(void* pArg)
{
	CAnimation* pAnim = new CAnimation();
	if (FAILED(pAnim->Initialize(pArg)))
	{
		Safe_Release(pAnim);
		MSG_BOX("CREATE FAIL : MODEL ANIMATION ");
	}
	return pAnim;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	__super::Free();

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);
	m_Channels.clear();
}
