#pragma once
#include "Engine_Defines.h"

namespace Client
{
	enum class ITEM_TYPE { EQUIPMENT, CONSUM, ETC, END };
	enum class WEAPON	 { MELEE, BOW, GUN, AXE, THROW, NONE, END };
	enum class EUQIP_TYPE { WEAPON, ARMOR, END };

	typedef struct EuqipItemDesc
	{
		_uInt				iAtkPoint;
		_uInt				iGuardPoint;
		_uInt				iHealthPoint;
		_bool				bIsLeftSocket;
		_bool				bIsChargeAble;
		EUQIP_TYPE			Equip_Type;
		WEAPON				Weapon_Type;
	}EUQIP_ITEM_DESC;

	typedef struct ConsumeItemDesc
	{
		_uInt				iEffectType;
		_uInt				iRecoveryPoint;
		_bool				bIsStateEffectHeal;
	}CONSUME_ITEM_DESC;

	typedef union ItemTypeDesc
	{
		CONSUME_ITEM_DESC ConsumDesc;
		EUQIP_ITEM_DESC	  EuqipDesc;

	}ITME_TYPE_DESC;

	typedef struct InitItem_Desc
	{
		unsigned int			iItemNum = {};

		WCHAR					szItemName[MAX_PATH] = {};
		WCHAR					szItemIconPath[MAX_PATH] = {};
		WCHAR					szItemModelPath[MAX_PATH] = {};
		ITEM_TYPE				ItemType = {};
		_bool					IsAnimModel = false;

		ITME_TYPE_DESC			TypeDesc;
	}ITEM_DESC;
}
