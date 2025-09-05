#include "PlayerSprint.h"

CPlayerSprintState::CPlayerSprintState(const char* szStateName) :
    CState(szStateName)
{
}

void CPlayerSprintState::OnStateEnter(void* pArg)
{
}

void CPlayerSprintState::OnStateExcution(void* pArg)
{
}

void CPlayerSprintState::OnStateExit(void* pArg)
{
}

CPlayerSprintState* CPlayerSprintState::Create(const char* szStateName)
{
    return new CPlayerSprintState(szStateName);
}

void CPlayerSprintState::Free()
{
}
