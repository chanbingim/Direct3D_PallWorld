#pragma once

#include "Engine_Defines.h"
#include "Client_Struct.h"
#include "PellSkillStruct.h"

NS_BEGIN(Client)

enum class PELL_TYPE { NORMAL, FIRE, WATER, ELETRIC, LEAF, END};
enum class PELL_WORK_TYPE { MAKE, COOK, TREE_GATHER, MINING, END };

typedef struct PellInfo : public CHARACTER_DESC
{
	const char*					szPellName;
	_float						fPellAttackRange;
	_float						fPellActTime;

	_float						fPellWalkSpeed;
	_float						fPellRunSpeed;

	PELL_SKILL_DATA				DefaultSkill;
	vector<PELL_SKILL_DATA>		PartnerSkillList;

	PELL_WORK_TYPE				eWorkType;
	PELL_TYPE					ePellType;
}PELL_INFO;

typedef struct Pell_Save_Data : public CHARACTER_SAVE_DESC
{
	_uInt				PellID;
	const char*			szPellName;
	_float				fPellAttackRange;
	_float				fPellActTime;

	_uInt						iDefaultSkillIndex;
	vector<PELL_SKILL_DATA>		PartnerSkillIndexList;

	PELL_WORK_TYPE		eWorkType;
	PELL_TYPE			ePellType;
}PELL_SAVE_DATA;

NS_END