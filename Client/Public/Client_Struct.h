#pragma once
#include "Engine_Defines.h"

namespace Client
{
	typedef struct Character_Desc
	{
		_uInt	iLevel;
		//플레이어의 기본데이터를 표시할 구조체
		// 체력
		float	MaxHealth, CurHealth;
		// 스테미나
		float	MaxStemina, CurStemina;
		// 배고픔
		float	MaxHunger, CurHunger;

		// 현재 쉴드가 가지고있는 방어게이지
		unsigned int	ShieldPoint, MaxShiledPoint;

		// 캐릭터의 공격력
		float	ATKPoint;
	}CHARACTER_DESC;

	typedef struct Character_Save_Defualt
	{
		// 체력
		float	MaxHealth;
		// 스테미나
		float	MaxStemina;
		// 배고픔
		float	MaxHunger;

		// 현재 쉴드가 가지고있는 방어게이지
		unsigned int	ShieldPoint;

		// 캐릭터의 공격력
		float	ATKPoint;
	}CHARACTER_SAVE_DESC;

	typedef struct Default_Damage_Desc
	{
		//Damaged
		_float					fDmaged;





	}DEFAULT_DAMAGE_DESC;
}