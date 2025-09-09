#include "PellAttackState.h"

CPellAttackState::CPellAttackState(const char* szStateName) :
    CState(szStateName)
{
}

void CPellAttackState::OnStateEnter(void* pArg)
{
}

void CPellAttackState::OnStateExcution(void* pArg)
{
}

void CPellAttackState::OnStateExit(void* pArg)
{
}

CPellAttackState* CPellAttackState::Create(const char* szStateName)
{
    return new CPellAttackState(szStateName);
}

void CPellAttackState::Free()
{
    __super::Free();
}
