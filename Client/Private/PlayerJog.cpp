#include "PlayerJog.h"

CPlayerJogState::CPlayerJogState(const char* szStateName) :
    CState(szStateName)
{
}

void CPlayerJogState::OnStateEnter(void* pArg)
{
}

void CPlayerJogState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CPlayerJogState::OnStateExit(void* pArg)
{
}

CPlayerJogState* CPlayerJogState::Create(const char* szStateName)
{
    return new CPlayerJogState(szStateName);
}

void CPlayerJogState::Free()
{
}
