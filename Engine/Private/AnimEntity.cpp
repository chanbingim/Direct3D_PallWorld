#include "AnimEntity.h"

HRESULT CAnimEntity::Initialize(void* pArg)
{
	if(pArg)
	{
		ANIM_DESC* Desc = static_cast<ANIM_DESC*>(pArg);
		m_fFrame = Desc->fFrame;
		m_bIsLoop = Desc->bIsLoop;
		m_fRateTime = Desc->fRateTime;

		m_fIncTime = m_fRateTime / m_fFrame;
	}

	return S_OK;
}

void CAnimEntity::Priority_Update()
{
}

void CAnimEntity::Update()
{
	auto iter = find_if(m_FrameKey.begin(), m_FrameKey.end(), [&](auto& pair)
		{
			return pair.first == m_fFrame;
		});

	if (iter != m_FrameKey.end())
		iter->second();

	m_bIsPlay = true;
	m_fCurFrame += m_fIncTime;

	if (m_fFrame <= m_fCurFrame)
	{
		if (m_bIsLoop)
			m_fCurFrame = 0.f;
		else
		{
			m_fCurFrame = m_fFrame;
			m_bIsPlay = false;
			m_bIsFinished = true;
		}
	}
}

void CAnimEntity::Late_Update()
{
}

HRESULT CAnimEntity::BindKeyFrame(_uInt Frame, function<void()> Funtor)
{
	if (m_fFrame < Frame || nullptr == Funtor)
		return E_FAIL;

	auto iter = find_if(m_FrameKey.begin(), m_FrameKey.end(), [&](auto& pair)
		{
			return pair.first == Frame;
		});

	if (iter == m_FrameKey.end())
		m_FrameKey.emplace_back(Frame, Funtor);
	else
		iter->second = Funtor;

	return S_OK;
}

const _bool& CAnimEntity::IsPlay()
{
	return m_bIsPlay;
}

_uInt CAnimEntity::GetCurFrame()
{
	return static_cast<_uInt>(m_fCurFrame);
}

void CAnimEntity::ReplayAnimation()
{
	m_fCurFrame = 0.0f;
}

const _bool& CAnimEntity::IsFinished()
{
	return m_bIsFinished;
}

void CAnimEntity::Free()
{
	__super::Free();
}
