#pragma once
#include "Engine_Defines.h"

namespace Client
{
	typedef struct Character_Desc
	{
		_float	MaxHealth, MaxStemina, MaxHunger, MaxWeight;
		_float	CurHealth, CurStemina, CurHunger, CurWeight;

		_float	ATKPoint;

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