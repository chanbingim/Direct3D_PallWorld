#include "State.h"

CState::CState(const char* szStateName)
{
	m_szStateName = szStateName;
}

void CState::OnStateEnter(void* pArg)
{
}

void CState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CState::OnStateExit(void* pArg)
{
}

_uInt CState::NexPhase()
{
	m_iPhaseIndex++;
	if (ENUM_CLASS(ACTION_PHASE::END) < m_iPhaseIndex)
		m_iPhaseIndex = 0;

	return m_iPhaseIndex;
}

void CState::Free()
{
}
