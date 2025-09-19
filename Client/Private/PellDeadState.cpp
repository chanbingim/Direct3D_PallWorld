#include "PellDeadState.h"

CPellDeadState::CPellDeadState(const char* szStateName) :
    CState(szStateName)
{ }

void CPellDeadState::OnStateEnter(void* pArg)
{
}

void CPellDeadState::OnStateExcution(void* pArg)
{
    switch (m_iPhaseIndex)
    {
    case 0 :
        m_szStateName = "Carrying_Start";
        break;
    case 1:
        m_szStateName = "Carrying";
        break;
    }
}

void CPellDeadState::OnStateExit(void* pArg)
{
}

CPellDeadState* CPellDeadState::Create(const char* szStateName)
{
    return new CPellDeadState(szStateName);;
}

void CPellDeadState::Free()
{
    __super::Free();
}
