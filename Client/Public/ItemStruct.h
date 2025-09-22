#pragma once
#include "Engine_Defines.h"

namespace Client
{
	enum class ITEM_TYPE { EQUIPMENT, CONSUM, ETC, END };
	enum class WEAPON	 { MELEE, AXE, BOW, GUN, THROW, NONE, END };
	enum class EUQIP_TYPE { WEAPON, ARMOR, END };

	typedef struct EuqipItemDesc
	{
		// 무기의 UI Icon 정보를 가진녀석
		WCHAR				ProjectilePrototpyeName[MAX_PATH];
		_uInt				iAtkPoint;
		_uInt				iGuardPoint;
		_uInt				iHealthPoint;
		_bool				bIsLeftSocket;
		_bool				bIsChargeAble;
		_Int				iProjectileIndex;

		//충돌체 정보
		_float3				vCenter;
		_float3				vExtents;

		EUQIP_TYPE			Equip_Type;
		WEAPON				Weapon_Type;
	}EUQIP_ITEM_DESC;

	typedef struct ConsumeItemDesc
	{
		// 보여줘야 한다면 사용할 변수들
		_bool				bIsLeftSocket;

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

		// 무기의 UI Icon 정보를 가진녀석
		WCHAR					szItemIconPath[MAX_PATH] = {};

		//무기의 모델정보를 가진 녀석
		WCHAR					szItemModelPath[MAX_PATH] = {}; 

		ITEM_TYPE				ItemType = {};

		_bool					IsAnimModel = false;
		_bool					IsPlayAnimation = false;

		ITME_TYPE_DESC			TypeDesc;
	}ITEM_DESC;
}
