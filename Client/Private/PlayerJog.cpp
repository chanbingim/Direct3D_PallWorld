#include "PlayerJog.h"

CPlayerJogState::CPlayerJogState()
{
}

void CPlayerJogState::OnEnterState(void* pArg)
{
}

void CPlayerJogState::PlayState(void* pArg)
{
}

void CPlayerJogState::OnEndState(void* pArg)
{
}

CPlayerJogState* CPlayerJogState::Create()
{
    return new CPlayerJogState();
}

void CPlayerJogState::Free()
{
}
