#include "CrouchState.h"

CCrouchState::CCrouchState(const char* szStateName) :
    CState(szStateName)
{
}

void CCrouchState::OnStateEnter(void* pArg)
{
}

void CCrouchState::OnStateExcution(void* pArg)
{
}

void CCrouchState::OnStateExit(void* pArg)
{
}

CCrouchState* CCrouchState::Create(const char* szStateName)
{
    return new CCrouchState(szStateName);
}

void CCrouchState::Free()
{
}
