#pragma once
#include "Engine_Defines.h"
#include "Client_Struct.h"

NS_BEGIN(Client)

enum class PELL_SKILL_TYPE { NORAML_COMBO, NORMAL, SPECIAL_COMBO, SPECIAL, END };
enum class PELL_SKILL_DAMAGE_TYPE { HIT, OVERLAP, END };

typedef struct Pal_Skill_Network_data
{
	_uInt				iSkillID;
	_char				szSkillName[MAX_PATH];

	// 나중에 어떤 이펙트를 사용하는지랑 넣어둘거같음
	// 설마 Far스킬이 파트너 스킬일수도있음

	_float					iSkillDelay;
	_uInt					iSkillDamage;
	_float					fSkillMoveSpeed;

	_float					fStartDurationTime;
	_float					fSkillDurationTime;
	_float					fAfterStateDelay;

	_bool						bIsProjectTile;
	PELL_SKILL_DAMAGE_TYPE		eSkillDamageType;
	_float						fTickDamageTime;

	PELL_SKILL_TYPE				eSkillType;
}PAL_SKILL_NETWORK_DATA;

NS_END