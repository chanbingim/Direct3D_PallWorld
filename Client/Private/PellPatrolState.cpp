#include "PellPatrolState.h"

#include "PellBase.h"
#include "Player.h"
#include "PlayerStateMachine.h"

#include "GameInstance.h"

CPellPatrolState::CPellPatrolState(const char* szStateName) :
	CState(szStateName)
{
}

void CPellPatrolState::OnStateEnter(void* pArg)
{
	if (nullptr == pArg)
	{
		m_eMoveType = PELL_MOVE_TYPE::WALK;
		m_szStateName = "Walk";
	}
	else
	{
		PELL_PATROL_STATE_DESC* PatrolDesc = static_cast<PELL_PATROL_STATE_DESC*>(pArg);
		m_pPlayer = PatrolDesc->pPlayer;
		m_pActPell = PatrolDesc->pActPell;
		auto PellInfoData = m_pActPell->GetPellInfo();

		m_bIsPartnerPell = PatrolDesc->bIsPartnerPell;
		m_pPellMoveSpeed = PatrolDesc->fPellMoveSpeed;

		if(false == m_bIsPartnerPell)
		{
			if (PatrolDesc->bIsCombat)
			{
				m_eMoveType = PELL_MOVE_TYPE::SPRINT;
				*PatrolDesc->fPellMoveSpeed = PellInfoData.fPellRunSpeed;
				m_szStateName = "Run";
			}
			else
			{
				_float fRandomNum = CGameInstance::GetInstance()->Random(0.f, 100.f);
				if (fRandomNum < 30.f)
				{
					m_eMoveType = PELL_MOVE_TYPE::SPRINT;
					*PatrolDesc->fPellMoveSpeed = PellInfoData.fPellRunSpeed;
					m_szStateName = "Run";
				}
				else
				{
					m_eMoveType = PELL_MOVE_TYPE::WALK;
					*PatrolDesc->fPellMoveSpeed = PellInfoData.fPellWalkSpeed;
					m_szStateName = "Walk";
				}
			}
		}
	}
}

void CPellPatrolState::OnStateExcution(_float fDeletaTime, void* pArg)
{
	if (m_bIsPartnerPell)
	{
		CPlayerStateMachine::PLAYER_STATE pState;
		m_pPlayer->GetPlayerState(&pState);

		switch (pState.eMove_Child_State)
		{
		case CPlayerStateMachine::MOVE_CHILD_ACTION::WALK:
		case CPlayerStateMachine::MOVE_CHILD_ACTION::JOG:
			m_szStateName = "Walk";
			*m_pPellMoveSpeed = P_JOG_SPEED - 1.5f;
			break;
		case CPlayerStateMachine::MOVE_CHILD_ACTION::SPRINT:
			m_szStateName = "Run";
			*m_pPellMoveSpeed = P_RUN_SPEED - 2.5f;
			break;
		}
	}
	else
	{
		auto PellInfoData = m_pActPell->GetPellInfo();
		switch (m_eMoveType)
		{
		case PELL_MOVE_TYPE::WALK : 
			*m_pPellMoveSpeed = PellInfoData.fPellWalkSpeed;
			m_szStateName = "Walk";
			break;
		case PELL_MOVE_TYPE::SPRINT:
			*m_pPellMoveSpeed = PellInfoData.fPellRunSpeed;
			m_szStateName = "Run";
			break;
		}
	}
}

void CPellPatrolState::OnStateExit(void* pArg)
{
	m_pActPell = nullptr;
	if(m_pPellMoveSpeed)
		*m_pPellMoveSpeed = P_WALK_SPEED;

	m_eMoveType = PELL_MOVE_TYPE::END;

	m_pPellMoveSpeed = nullptr;
	m_bIsPartnerPell = false;
	m_pPlayer = nullptr;
}

CPellPatrolState* CPellPatrolState::Create(const char* szStateName)
{
	return new CPellPatrolState(szStateName);
}

void CPellPatrolState::Free()
{
	__super::Free();
}
