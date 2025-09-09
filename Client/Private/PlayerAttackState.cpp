#include "PlayerAttackState.h"

CPlayerAttackState::CPlayerAttackState(const char* szStateName) :
	CState(szStateName)
{
}

void CPlayerAttackState::OnStateEnter(void* pArg)
{
	m_iPhaseIndex = 0;
	m_szStateName = "AttackCharge";
}

void CPlayerAttackState::OnStateExcution(void* pArg)
{
	ChangeStateName();
}

void CPlayerAttackState::OnStateExit(void* pArg)
{
}

void CPlayerAttackState::ChangeStateName()
{
	switch (m_iPhaseIndex)
	{
	case 0:
		m_szStateName = "AttackCharge";
		break;
	case 1:
		m_szStateName = "AttackChargeLoop";
		break;
	}
}

CPlayerAttackState* CPlayerAttackState::Create(const char* szStateName)
{
	return new CPlayerAttackState(szStateName);
}

void CPlayerAttackState::Free()
{
	__super::Free();
}
