#include "HitState.h"

CHitState::CHitState()
{
}

void CHitState::OnEnterState(void* pArg)
{
}

void CHitState::PlayState(void* pArg)
{
}

void CHitState::OnEndState(void* pArg)
{
}

CHitState* CHitState::Create()
{
    return new CHitState();
}

void CHitState::Free()
{
}
