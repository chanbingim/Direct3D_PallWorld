#pragma once
#include "Engine_Defines.h"

namespace Engine
{
	// 메모리 구조상 자동으로 안정성이 확보되는게 아니기때문에
	// UNION 을 사용하게되면 조심하자
	// 공용체를 통해서 메모리 공간을 아끼면서 들어가자
	// 또한 Type을 통해 내가 원하는 녀석들만 파싱이 될수있다.
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