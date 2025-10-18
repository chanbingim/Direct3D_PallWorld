#pragma once
#include "Engine_Defines.h"

namespace Engine
{
	typedef struct SaveLevelDesc
	{
		_uInt				eType;
		_uInt				iPrototypeLevelID;
		_uInt				iSaveLevelID;
		_char				PrototypeName[MAX_PATH];
		_char				LayerName[MAX_PATH];
		_uInt				PrototypeIndex;
		_float3				vScale;
		_float3				vRotation;
		_float3				vPosition;
	}SAVE_LEVEL_DESC;
}