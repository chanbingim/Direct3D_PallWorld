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

		static constexpr unsigned int			  iNumElements = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[] = {
		   {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTX_TEX;

	typedef struct tagVertexNorTex
	{
		_float3		vPosition;
		_float3		vNormal;
		_float2		vTexcoord;

		static constexpr unsigned int			  iNumElements = { 3 };
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTX_NORTEX;

	typedef struct tagVertexMesh
	{
		_float3		vPosition;
		_float3		vNormal;
		_float3		vTangent;
		_float2		vTexcoord;

		static constexpr unsigned int			  iNumElements = { 4 };
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTX_MESH;

	typedef struct tagVertexAnimationMesh
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT3			vTangent;
		XMFLOAT2			vTexcoord;
		XMUINT4				vBlendIndex;
		XMFLOAT4			vBlendWeight;

		static constexpr unsigned int					iNumElements = { 6 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VTX_ANIM_MESH;

	typedef struct tagVertexColor
	{
		_float3		vPosition;

		static constexpr unsigned int					iNumElements = { 1 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTX_COL;

	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vTranslation;
		float			fTrackPosition;
	}KEYFRAME;

}
