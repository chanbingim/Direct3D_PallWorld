#pragma once
#include "Engine_Defines.h"
#include "Client_Struct.h"

NS_BEGIN(Client)

enum class PELL_SKILL_TYPE { NORAML, SPECIAL, END };

typedef struct Pell_Skill_Data
{
	const char*			SkillName;

	_uInt				iSkillDamage;
	_float				fAfterStateDelay;

	_float				fStartDurationTime;
	_float				fSkillDurationTime;

	PELL_SKILL_TYPE		eSkillType;
}PELL_SKILL_DATA;

typedef struct Pell_Save_Skill_Data
{
	_uInt				iSkillID;
	const char*			SkillName;

	// 나중에 어떤 이펙트를 사용하는지랑 넣어둘거같음
	// 설마 Far스킬이 파트너 스킬일수도있음

	_uInt				iSkillDamage;
	_float				fAfterStateDelay;

	_float				fSkillDelay;
	_float				fSkillRange;

	PELL_SKILL_TYPE		eSkillType;
}PELL_SAVE_SKILL_DATA;

NS_END