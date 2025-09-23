#pragma once

#include "Engine_Defines.h"
#include "Client_Struct.h"
#include "PellSkillStruct.h"


NS_BEGIN(Engine)
class CTexture;
NS_END

NS_BEGIN(Client)

enum class PELL_TYPE { NORMAL, FIRE, WATER, ELETRIC, LEAF, END};
enum class PELL_WORK_TYPE { MAKE, COOK, TREE_GATHER, MINING, END };
enum class PELL_STORAGE_STATE { WORLD, PARTNER_PELL, PLAYER_INVEN, PELL_SHPER, PELL_BOX, END };

typedef struct PellInfo : public CHARACTER_DESC
{
	const char*					szPellName;
	const CTexture*				pPellIconTexture;

	_float						fPellAttackRange;
	_float						fPellActTime;

	_float						fPellWalkSpeed;
	_float						fPellRunSpeed;

	PELL_SKILL_DATA				DefaultSkill;
	vector<PELL_SKILL_DATA>		PartnerSkillList;

	PELL_WORK_TYPE				eWorkType;
	PELL_TYPE					ePellType;
	PELL_STORAGE_STATE			ePellStorageState;
}PELL_INFO;

typedef struct Pell_Save_Data : public CHARACTER_SAVE_DESC
{
	_uInt				PellID;
	const char*			szPellName;
	const char*			szPellIconPath;

	_float				fPellAttackRange;
	_float				fPellActTime;

	_uInt						iDefaultSkillIndex;
	vector<PELL_SKILL_DATA>		PartnerSkillIndexList;

	PELL_WORK_TYPE		eWorkType;
	PELL_TYPE			ePellType;
}PELL_SAVE_DATA;

NS_END