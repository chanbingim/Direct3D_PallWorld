#include "PlayerHitState.h"
#include "PlayerManager.h"

CPlayerHitState::CPlayerHitState(const char* szStateName) :
    CState(szStateName)
{
}

void CPlayerHitState::OnStateEnter(void* pArg)
{








}

void CPlayerHitState::OnStateExcution(_float fDeletaTime, void* pArg)
{



}

void CPlayerHitState::OnStateExit(void* pArg)
{
}

void CPlayerHitState::ChangeStateName()
{
}

CPlayerHitState* CPlayerHitState::Create(const char* szStateName)
{
    return new CPlayerHitState(szStateName);
}

void CPlayerHitState::Free()
{

}
