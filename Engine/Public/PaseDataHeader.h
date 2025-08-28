#pragma once
#include "Engine_Defines.h"

namespace Engine
{
	typedef struct SaveLevelDesc
	{


	}SAVE_LEVEL_DESC;

	typedef struct SaveBoneDesc
	{
		_char				szName[MAX_PATH];
		_float4x4			TransformationMatrix;
		_Int				iParentBoneIndex;
	}SAVE_BONE_DESC;

	typedef struct SaveChannelDesc
	{





	}SAVE_CHANNEL_DESC;


	typedef struct SaveAnimationDesc
	{





	}SAVE_ANIMATION_DESC;


	typedef struct SaveMeshDesc
	{
		_char				szName[MAX_PATH];
		unsigned int		iNumVertices;
		unsigned int		iNumMaterialIndex;
		unsigned int		iNumFaces;

		vector<VTX_MESH>	Vertices;
		vector<_uInt>		Indices;
	}SAVE_MESH_DESC;

	typedef struct SaveAnimMeshDesc
	{
		_char					szName[MAX_PATH];
		unsigned int			iNumVertices;
		unsigned int			iNumMaterialIndex;
		unsigned int			iNumFaces;
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
		vector<MATRIAL_TEXTURE_TYPE_DESC>  TextureType;
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
	}SAVE_ANIM_MODEL_DESC;
}