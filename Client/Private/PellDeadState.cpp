#include "PellDeadState.h"

CPellDeadState::CPellDeadState(const char* szStateName) :
    CState(szStateName)
{
    m_iLastPhase = 1;
}

void CPellDeadState::OnStateEnter(void* pArg)
{
}

void CPellDeadState::OnStateExcution(_float fDeletaTime, void* pArg)
{
    switch (m_iPhaseIndex)
    {
    case 0 :
        m_szStateName = "Carrying_Start";
        m_bStateAnimLoop = false;
        break;
    case 1:
        m_szStateName = "Carrying";
        m_bStateAnimLoop = true;
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
