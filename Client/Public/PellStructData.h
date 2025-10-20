#pragma once

#include "Engine_Defines.h"
#include "Client_Struct.h"
#include "PellSkillStruct.h"


NS_BEGIN(Engine)
class CTexture;
NS_END

NS_BEGIN(Client)

enum class PELL_TYPE { NORMAL, FIRE, WATER, ELETRIC, LEAF, END};
enum class PELL_WORK_TYPE { COMMONE_WORK, TRANSPORT, MINING, COOK, TREE_GATHER, END };
enum class PELL_STORAGE_STATE { WORLD, PARTNER_PELL, PLAYER_AREA, PLAYER_INVEN, PELL_SHPER, PELL_BOX, END };

typedef struct PalCopyData : public CHARACTER_DESC
{
	_uInt						iPellID;
	WCHAR						szPellName[MAX_PATH];
	WCHAR						szPrototyeName[MAX_PATH];

	_float						fPellAttackRange;
	_float						fPellActTime;

	_float						fPellWalkSpeed;
	_float						fPellRunSpeed;

	_uInt						iDefaultSkillIndex;
	
	PELL_WORK_TYPE				eWorkType;
	PELL_TYPE					ePellType;
}PAL_NETWORK_DATA;


typedef struct PellInfo : public PAL_NETWORK_DATA
{
	_wstring							NickName;
	const CTexture*						pPellIconTexture;

	PAL_SKILL_NETWORK_DATA				DefaultSkill;
	vector<PAL_SKILL_NETWORK_DATA>		PartnerSkillList;

	PELL_STORAGE_STATE					ePellStorageState;
}PELL_INFO;

NS_END