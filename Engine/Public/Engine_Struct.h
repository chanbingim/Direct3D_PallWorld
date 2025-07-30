#pragma once
#include "Engine_Types.h"

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HWND				hWnd;
		HINSTANCE			Instance;
		_uInt				iNumLevels;
		_uInt				iWinSizeX, iWinSizeY;
		_bool				bIsWindowed;
	}ENGINE_DESC;

	typedef struct tagVertexTex
	{
		_float3		vPosition;
		_float2		vTexcoord;
	}VTX_TEX;

	typedef struct tagVertexColor
	{
		_float3		vPosition;
		_float4		vColor;
	}VTX_COL;
}
