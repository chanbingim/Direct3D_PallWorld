#include "PellStateStun.h"

CPellStateStun::CPellStateStun(const char* szStateName) :
    CState(szStateName)
{
    m_iLastPhase = 1;
}

void CPellStateStun::OnStateEnter(void* pArg)
{
    if (nullptr == pArg)
    {

    }
    else
    {
         PELL_STATE_STUN_STATE* pDesc = static_cast<PELL_STATE_STUN_STATE*>(pArg);
        if (pDesc)
        {
            m_ActionFunc = pDesc->ActionFunction;
            m_fEndStunTime = pDesc->StunTime;
        }
    }

    m_fAccStunTime = 0.0f;
}

void CPellStateStun::OnStateExcution(_float fDeletaTime, void* pArg)
{
    if(!m_bIsStun)
        m_fAccStunTime += fDeletaTime;

    if (m_fAccStunTime >= m_fEndStunTime)
    {
        m_bIsStun = true;
        m_iPhaseIndex++;
    }

    m_ActionFunc(fDeletaTime, m_bIsStun);
    switch (m_iPhaseIndex)
    {
    case 0 :
        m_szStateName = "SleepLoop";
        break;
    case 1:
        m_szStateName = "SleepEnd";
        break;
    }
}

void CPellStateStun::OnStateExit(void* pArg)
{
    m_ActionFunc = nullptr;
    m_iPhaseIndex = 0;
    m_fAccStunTime = 0.f;
}

CPellStateStun* CPellStateStun::Create(const char* szStateName)
{
    return new CPellStateStun(szStateName);
}

void CPellStateStun::Free()
{
    __super::Free();
}
