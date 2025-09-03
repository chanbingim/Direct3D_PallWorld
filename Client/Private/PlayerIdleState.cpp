#include "PlayerIdleState.h"

CPlayerIdleState::CPlayerIdleState()
{
}

void CPlayerIdleState::OnEnterState(void* pArg)
{
}

void CPlayerIdleState::PlayState(void* pArg)
{
}

void CPlayerIdleState::OnEndState(void* pArg)
{
}

CPlayerIdleState* CPlayerIdleState::Create()
{
    return new CPlayerIdleState();
}

void CPlayerIdleState::Free()
{
}
