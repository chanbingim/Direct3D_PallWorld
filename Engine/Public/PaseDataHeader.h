#pragma once
#include "Engine_Defines.h"

namespace Engine
{
	typedef struct SaveMeshDesc
	{
		unsigned int	iNumVertices;
		unsigned int	iNumMaterialIndex;
		unsigned int	iNumFaces;

		vector<VTX_MESH>	Vertices;
		vector<_uInt>		Indices;
	}SAVE_MESH_DESC;

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
}