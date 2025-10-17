#include "ClimbState.h"

CClimbState::CClimbState(const char* szStateName) :
    CState(szStateName)
{
}

void CClimbState::OnStateEnter(void* pArg)
{
}

void CClimbState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CClimbState::OnStateExit(void* pArg)
{
}

CClimbState* CClimbState::Create(const char* szStateName)
{
    return new CClimbState(szStateName);
}

void CClimbState::Free()
{
}
