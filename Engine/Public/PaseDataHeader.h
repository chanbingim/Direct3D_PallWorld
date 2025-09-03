#pragma once
#include "Engine_Defines.h"

namespace Engine
{
	enum class SAVE_OBJECT_TYPE { CAMERA, TERRAIN, OBEJCT, ENVIORNMENT, END };
	typedef struct SaveTerrianDesc
	{
		_uInt			TerrainType;
		_float2			TileCnt;
		_char			HeightMapCom[MAX_PATH];
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
		SAVE_OBJECT_TYPE	eType;
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

	typedef struct SaveBoneDesc
	{
		_char				szName[MAX_PATH];
		_float4x4			PreTransformMat;
		_Int				iParentBoneIndex;
		_Int				iChildCnt;
	}SAVE_BONE_DESC;

	typedef struct SaveChannelDesc
	{
		_char					szName[MAX_PATH] = {};
		_Int					iBoneIndex = { -1 };

		_uInt					iNumKeyFrames = {};
		vector<KEYFRAME>		KeyFrames;
	}SAVE_CHANNEL_DESC;


	typedef struct SaveAnimationDesc
	{
		_char						szAnimName[MAX_PATH];

		/* 애니메이션 재생을 위한 전체 길이 */
		_float						fLength = {};

		/* 초당 얼마나 재생되어야하는지 : 재생 속도 */
		_float						fTickPerSecond = {};
		_bool						bIsLoop = false;

		_uInt						iNumChannels = {};
		vector<SAVE_CHANNEL_DESC>	Channels;
	}SAVE_ANIMATION_DESC;


	typedef struct SaveMeshDesc
	{
		_char				szName[MAX_PATH];
		unsigned int		iNumVertices;
		unsigned int		iNumFaces;
		unsigned int		iNumMaterialIndex;

		vector<VTX_MESH>	Vertices;
		vector<_uInt>		Indices;
	}SAVE_MESH_DESC;

	typedef struct SaveAnimMeshDesc
	{
		_char					szName[MAX_PATH];
		unsigned int			iNumVertices;
		unsigned int			iNumFaces;
		unsigned int			iNumMaterialIndex;
		unsigned int			iNumBones;

		vector<VTX_ANIM_MESH>	Vertices;
		vector<_uInt>			Indices;

		vector<_uInt>			BoneIndices;
		vector<_float4x4>		OffsetMatrices;
	}SAVE_ANIM_MESH_DESC;

	typedef	struct MatrialTextureTypeDesc
	{
		unsigned int		iNumShaderResourceView;
		vector<_wstring>	TexturePath;
	}MATRIAL_TEXTURE_TYPE_DESC;

	typedef struct SaveMatrialDesc
	{
		unsigned int						iTextureTypeMax;
		vector<MATRIAL_TEXTURE_TYPE_DESC>   TextureType;
	}SAVE_MATERIAL_DESC;

	typedef struct SaveModelDesc
	{
		unsigned int				iNumMeshes;
		vector<SAVE_MESH_DESC>		MeshDesc;

		unsigned int				iNumMaterials;
		vector<SAVE_MATERIAL_DESC>	MatrialDesc;
	}SAVE_MODEL_DESC;

	typedef struct SaveAnimModelDesc
	{
		unsigned int				iNumMeshes;
		vector<SAVE_ANIM_MESH_DESC>	MeshDesc;

		unsigned int				iNumMaterials;
		vector<SAVE_MATERIAL_DESC>	MatrialDesc;

		unsigned int				iNumBones;
		vector<SAVE_BONE_DESC>		BoneDesc;

		unsigned int				iNumAnimations;
		vector<SAVE_ANIMATION_DESC>	AnimationDesc;
	}SAVE_ANIM_MODEL_DESC;
}