#include "PellResetState.h"

CPellResetState::CPellResetState(const char* szStateName) :
    CState(szStateName)
{
}

void CPellResetState::OnStateEnter(void* pArg)
{
}

void CPellResetState::OnStateExcution(void* pArg)
{
}

void CPellResetState::OnStateExit(void* pArg)
{
}

CPellResetState* CPellResetState::Create(const char* szStateName)
{
    return new CPellResetState(szStateName);
}

void CPellResetState::Free()
{
    __super::Free();
}
