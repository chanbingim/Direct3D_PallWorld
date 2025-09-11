#pragma once

#include "Engine_Defines.h"

namespace Engine
{
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