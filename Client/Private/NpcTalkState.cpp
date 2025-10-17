#include "NpcTalkState.h"

#include "wchar.h"
#include "GameInstance.h"

CNpcTalkState::CNpcTalkState(const char* szStateName) :
    CState(szStateName)
{
}

void CNpcTalkState::OnStateEnter(void* pArg)
{
    m_iTalkIndex = CGameInstance::GetInstance()->Random(0, 4);

    m_szStateName = "Talking";
    m_szStateName += m_iTalkIndex;
}

void CNpcTalkState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CNpcTalkState::OnStateExit(void* pArg)
{
}

CNpcTalkState* CNpcTalkState::Create(const char* szStateName)
{
    return new CNpcTalkState(szStateName);
}

void CNpcTalkState::Free()
{
}
