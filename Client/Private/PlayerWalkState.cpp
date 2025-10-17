#include "PlayerWalkState.h"

CPlayerWalkState::CPlayerWalkState(const char* szStateName) :
    CState(szStateName)
{
}

void CPlayerWalkState::OnStateEnter(void* pArg)
{
}

void CPlayerWalkState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CPlayerWalkState::OnStateExit(void* pArg)
{
}

CPlayerWalkState* CPlayerWalkState::Create(const char* szStateName)
{
    return new CPlayerWalkState(szStateName);
}

void CPlayerWalkState::Free()
{
}
