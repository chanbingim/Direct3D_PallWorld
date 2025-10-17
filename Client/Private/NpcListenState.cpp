#include "NpcListenState.h"

CNpcListenState::CNpcListenState(const char* szStateName) :
    CState(szStateName)
{
}

void CNpcListenState::OnStateEnter(void* pArg)
{
    m_szStateName = "Listen";
}

void CNpcListenState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CNpcListenState::OnStateExit(void* pArg)
{
}

CNpcListenState* CNpcListenState::Create(const char* szStateName)
{
    return new CNpcListenState(szStateName);
}

void CNpcListenState::Free()
{
    __super::Free();
}
