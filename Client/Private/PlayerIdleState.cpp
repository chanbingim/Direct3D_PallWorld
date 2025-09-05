#include "PlayerIdleState.h"

CPlayerIdleState::CPlayerIdleState(const char* szStateName) : 
    CState(szStateName)
{
}

void CPlayerIdleState::OnStateEnter(void* pArg)
{
}

void CPlayerIdleState::OnStateExcution(void* pArg)
{
}

void CPlayerIdleState::OnStateExit(void* pArg)
{
}

CPlayerIdleState* CPlayerIdleState::Create(const char* szStateName)
{
    return new CPlayerIdleState(szStateName);
}

void CPlayerIdleState::Free()
{
}
