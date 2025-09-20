#include "PlayerChildIdleState.h"

CPlayerChildIdleState::CPlayerChildIdleState(const char* szStateName) :
    CState(szStateName)
{
}

void CPlayerChildIdleState::OnStateEnter(void* pArg)
{
}

void CPlayerChildIdleState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CPlayerChildIdleState::OnStateExit(void* pArg)
{
}

CPlayerChildIdleState* CPlayerChildIdleState::Create(const char* szStateName)
{
    return new CPlayerChildIdleState(szStateName);
}

void CPlayerChildIdleState::Free()
{
}