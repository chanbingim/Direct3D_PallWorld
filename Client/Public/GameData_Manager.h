#pragma once

#include "Base.h"
#include "Client_Define.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CGameData_Manager : public CBase
{
	DECLARE_SINGLETON(CGameData_Manager);

private :
	CGameData_Manager() = default; 
	virtual ~CGameData_Manager() = default;

public :
	void					Initialize();

	PLAYER_DESC*			GetPlayerInfo() { return m_PlayerInfo; }

private :
	PLAYER_DESC*			m_PlayerInfo = nullptr;

public :
	virtual		void		Free() override;

};
NS_END