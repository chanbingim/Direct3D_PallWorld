#include "NpcIdleState.h"

CNpcIdleState::CNpcIdleState(const char* szStateName) :
    CState(szStateName)
{
}

void CNpcIdleState::OnStateEnter(void* pArg)
{
    m_szStateName = "Idle";
}

void CNpcIdleState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CNpcIdleState::OnStateExit(void* pArg)
{
}

CNpcIdleState* CNpcIdleState::Create(const char* szStateName)
{
    return new CNpcIdleState(szStateName);
}

void CNpcIdleState::Free()
{
    __super::Free();
}
