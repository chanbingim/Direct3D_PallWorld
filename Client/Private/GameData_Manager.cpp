#include "GameData_Manager.h"

IMPLEMENT_SINGLETON(CGameData_Manager);

void CGameData_Manager::Initialize()
{
	m_PlayerInfo = new PLAYER_DESC;

	//임시 데이터 세팅
	m_PlayerInfo->MaxHealth = 2000.f;
	m_PlayerInfo->MaxStemina = 200.f;
	m_PlayerInfo->MaxWeight = 1000.f;

	//여게서 캐릭터 매니저한테 요청해서 데이터 가져와서 초기 데이터 세팅
}

void CGameData_Manager::Free()
{
	Safe_Delete(m_PlayerInfo);
}
