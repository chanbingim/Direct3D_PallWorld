#include "PlayerWorkState.h"

#include "Architecture.h"
#include "PlayerPartData.h"

CPlayerWorkState::CPlayerWorkState(const char* szStateName) :
    CState(szStateName)
{
    m_iLastPhase = 2;
}

void CPlayerWorkState::OnStateEnter(void* pArg)
{
    PLAYER_WORK_STATE* pWorkStateDesc = static_cast<PLAYER_WORK_STATE*>(pArg);
    m_pArchitecture = pWorkStateDesc->pArchitecture;
    m_pPlayerAnim = pWorkStateDesc->pPlayerAnim;
    m_bIsAnimLoop = pWorkStateDesc->bIsAnimLoop;
    m_iPhaseIndex = 0;

}

void CPlayerWorkState::OnStateExcution(_float fDeletaTime, void* pArg)
{
    NextPhaseMove();

    switch (m_iPhaseIndex)
    {
    case 0 :
        m_szStateName = "Architecture2_Start";
        (*m_bIsAnimLoop) = false;
        break;
    case 1:
        m_szStateName = "Architecture2";
        (*m_bIsAnimLoop) = true;
        break;
    case 2:
        m_szStateName = "Architecture2_End";
        (*m_bIsAnimLoop) = false;
        break;
    }
}

void CPlayerWorkState::OnStateExit(void* pArg)
{
    m_pArchitecture = nullptr;
    (*m_bIsAnimLoop) = true;
    m_bIsAnimLoop = nullptr;
}

void CPlayerWorkState::NextPhaseMove()
{
    _bool bIsNext = false;
    if (1 == m_iPhaseIndex)
    {
        if (m_pArchitecture->IsBuildFinished())
            bIsNext = true;
    }
    else if(0 == m_iPhaseIndex)
    {
        if (m_pPlayerAnim->IsAnimFinished())
            bIsNext = true;
    }

    if (bIsNext)
        m_iPhaseIndex++;
}

CPlayerWorkState* CPlayerWorkState::Create(const char* szStateName)
{
    return new CPlayerWorkState(szStateName);
}

void CPlayerWorkState::Free()
{
    __super::Free();
}
