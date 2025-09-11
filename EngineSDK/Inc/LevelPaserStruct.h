#pragma once
#include "Engine_Defines.h"

namespace Engine
{
	enum class SAVE_OBJECT_TYPE { CAMERA = 0, TERRAIN, OBEJCT, ENVIORNMENT, END };
	typedef struct SaveTerrianDesc
	{
		_uInt			TerrainType;
		_float2			TileCnt;
		_char			HeightMapCom[MAX_PATH];
		_char			NaviMeshPath[MAX_PATH];
	}SAVE_TERRIAN_DESC;

	typedef struct SaveCameraDesc
	{
		_uInt			CameraType;
		_float			fNear;
		_float			fFar;
		_float			fFov;
	}SAVE_CAMERA_DESC;

	typedef union Save_Obejct_Desc
	{
		// Not use Two Desc
		// Use Only One
		SaveTerrianDesc		TerrianDesc;
		SaveCameraDesc		CameraDesc;
	}SAVE_OBJECT_DESC;

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

		SAVE_OBJECT_DESC	ObjectDesc;
	}SAVE_LEVEL_DESC;

}