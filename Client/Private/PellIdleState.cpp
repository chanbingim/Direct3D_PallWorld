#include "PellIdleState.h"

CPellIdleState::CPellIdleState(const char* szStateName) :
    CState(szStateName)
{
}

void CPellIdleState::OnStateEnter(void* pArg)
{
}

void CPellIdleState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CPellIdleState::OnStateExit(void* pArg)
{
}

CPellIdleState* CPellIdleState::Create(const char* szStateName)
{
    return new CPellIdleState(szStateName);
}

void CPellIdleState::Free()
{
    __super::Free();
}
