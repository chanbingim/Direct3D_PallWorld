#include "PlayerGrabState.h"

CPlayerGrabState::CPlayerGrabState(const char* szStateName) :
    CState(szStateName)
{
}

void CPlayerGrabState::OnStateEnter(void* pArg)
{

}

void CPlayerGrabState::OnStateExcution(_float fDeletaTime, void* pArg)
{

}

void CPlayerGrabState::OnStateExit(void* pArg)
{

}

CPlayerGrabState* CPlayerGrabState::Create(const char* szStateName)
{
    return new CPlayerGrabState(szStateName);
}

void CPlayerGrabState::Free()
{
    __super::Free();
}
