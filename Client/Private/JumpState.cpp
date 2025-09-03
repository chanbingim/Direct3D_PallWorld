#include "JumpState.h"

CJumpState::CJumpState()
{
}

void CJumpState::OnEnterState(void* pArg)
{
}

void CJumpState::PlayState(void* pArg)
{
}

void CJumpState::OnEndState(void* pArg)
{
}

CJumpState* CJumpState::Create()
{
    return new CJumpState();
}

void CJumpState::Free()
{
}
