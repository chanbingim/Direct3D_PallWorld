#include "PellPatrolState.h"

#include "PellBase.h"
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
		m_pActPell = PatrolDesc->pActPell;
		auto PellInfoData = m_pActPell->GetPellInfo();

		if (PatrolDesc->bIsPartnerPell)
		{
			m_eMoveType = PatrolDesc->ePellMoveType;
			switch (m_eMoveType)
			{
			case Client::CPellPatrolState::WALK:
				*PatrolDesc->fPellMoveSpeed = PellInfoData.fPellWalkSpeed;
				m_szStateName = "Walk";
				break;
			case Client::CPellPatrolState::SPRINT:
				*PatrolDesc->fPellMoveSpeed = PellInfoData.fPellRunSpeed;
				m_szStateName = "Sprint";
				break;
			}
		}
		else
		{
			_float fRandomNum = CGameInstance::GetInstance()->Random(0.f, 100.f);
			if (fRandomNum < 30.f)
			{
				m_eMoveType = PELL_MOVE_TYPE::SPRINT;
				*PatrolDesc->fPellMoveSpeed = PellInfoData.fPellRunSpeed;
				m_szStateName = "Sprint";
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

void CPellPatrolState::OnStateExcution(_float fDeletaTime, void* pArg)
{

}

void CPellPatrolState::OnStateExit(void* pArg)
{
	m_pActPell = nullptr;
	m_eMoveType = PELL_MOVE_TYPE::END;
}

CPellPatrolState* CPellPatrolState::Create(const char* szStateName)
{
	return new CPellPatrolState(szStateName);
}

void CPellPatrolState::Free()
{
	__super::Free();
}
