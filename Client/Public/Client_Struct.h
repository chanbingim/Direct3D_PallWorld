#pragma once
#include "Engine_Defines.h"

namespace Client
{
	typedef struct Character_Desc
	{
		//플레이어의 기본데이터를 표시할 구조체
		// 체력
		float	MaxHealth, CurHealth;
		// 스테미나
		float	MaxStemina, CurStemina;
		// 배고픔
		float	MaxHunger, CurHunger;

		// 현재 쉴드가 가지고있는 방어게이지
		unsigned int	ShieldPoint;

		// 캐릭터의 공격력
		float	ATKPoint;
	}CHARACTER_DESC;

	typedef struct Player_Desc : CHARACTER_DESC
	{
		_uInt					Stats[5];
		_uInt					EquipSlot[5];
		vector<_uInt>			ItemInven;
		list<_uInt>				ST_Effects;

	}PLAYER_DESC;

	typedef struct Pell_Desc : public CHARACTER_DESC
	{
		
	}PELL_DESC;
}