#include "Animation.h"

#include "PaseDataHeader.h"
#include "Channel.h"
#include "Model.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(const CModel* pModel, const aiAnimation* pAIAnimation, _bool bIsLoop)
{
	strcpy_s(m_szAnimName, pAIAnimation->mName.data);

	m_bIsLoop = bIsLoop;
	m_fLength = (_float)pAIAnimation->mDuration;
	m_fTickPerSecond = (_float)pAIAnimation->mTicksPerSecond;
	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uInt i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(pModel, pAIAnimation->mChannels[i]);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}
	return S_OK;
}

void CAnimation::UpdateTransformationMatrices(vector<CBone*>& Bones, _float fTimeDelta)
{
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;
	if (m_bIsLoop)
	{
		if (m_fCurrentTrackPosition > m_fLength)
			m_fCurrentTrackPosition = 0;
	}
	else
		m_fCurrentTrackPosition = m_fLength;

	for (auto& pChannel : m_Channels)
		pChannel->Update_TransformationMatrix(Bones, m_fCurrentTrackPosition);
}

_bool CAnimation::CompareAnimName(const char* szName)
{
	return !strcmp(szName, m_szAnimName);
}

void CAnimation::Export(void* pAnimationDesc)
{
	SAVE_ANIMATION_DESC* AnimDesc = static_cast<SAVE_ANIMATION_DESC *>(pAnimationDesc);

	strcpy_s(AnimDesc->szAnimName, m_szAnimName);
	AnimDesc->fLength = m_fLength;
	AnimDesc->fTickPerSecond = m_fTickPerSecond;
	AnimDesc->iNumChannels = m_iNumChannels;
	AnimDesc->bIsLoop = m_bIsLoop;
	
	AnimDesc->Channels.reserve(m_iNumChannels);
	for (_uInt i = 0; i < m_iNumChannels; ++i)
	{
		SAVE_CHANNEL_DESC ChannelDesc = {};
		m_Channels[i]->Export(&ChannelDesc);

		AnimDesc->Channels.push_back(ChannelDesc);
	}
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

void CAnimation::Free()
{
	__super::Free();

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);
	m_Channels.clear();
}
