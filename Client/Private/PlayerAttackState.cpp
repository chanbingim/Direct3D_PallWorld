#include "PlayerAttackState.h"

CPlayerAttackState::CPlayerAttackState() :
    CState()
{
}

void CPlayerAttackState::OnEnterState(void* pArg)
{
}

void CPlayerAttackState::PlayState(void* pArg)
{
}

void CPlayerAttackState::OnEndState(void* pArg)
{
}

CPlayerAttackState* CPlayerAttackState::Create()
{
    return new CPlayerAttackState();
}

void CPlayerAttackState::Free()
{
    __super::Free();
}
