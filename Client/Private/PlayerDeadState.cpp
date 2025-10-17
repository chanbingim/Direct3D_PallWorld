#include "PlayerDeadState.h"

CPlayerDeadState::CPlayerDeadState(const char* szStateName) :
    CState(szStateName)
{
}

void CPlayerDeadState::OnStateEnter(void* pArg)
{
}

void CPlayerDeadState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CPlayerDeadState::OnStateExit(void* pArg)
{
}

CPlayerDeadState* CPlayerDeadState::Create(const char* szStateName)
{
    return new CPlayerDeadState(szStateName);
}

void CPlayerDeadState::Free()
{
}
