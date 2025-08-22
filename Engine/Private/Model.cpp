#include "Model.h"

#include "GameInstance.h"
#include "Mesh.h"
#include "Material.h"
#include "StringHelper.h"
#include "PaseDataHeader.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel& rhs) :
	CComponent(rhs),
	m_Meshes(rhs.m_Meshes),
	m_iNumMeshes(rhs.m_iNumMeshes),
	m_iNumMaterials(rhs.m_iNumMaterials),
	m_Materials(rhs.m_Materials),
	m_eType(rhs.m_eType)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pMaterial : m_Materials)
		Safe_AddRef(pMaterial);
}

HRESULT CModel::Initialize_Prototype(MODEL_TYPE eType, const _char* pModelFilePath)
{
	_char		szEXT[MAX_PATH] = {};

	_splitpath_s(pModelFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);
	m_eType = eType;

	if (!strcmp(szEXT, ".fbx"))
	{
		_uInt iFlags = {};

		iFlags = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
		if (MODEL_TYPE::NONANIM == m_eType)
			iFlags |= aiProcess_PreTransformVertices;
		
		m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlags);
		if (nullptr == m_pAIScene)
			return E_FAIL;

		if (FAILED(Ready_Meshes()))
			return E_FAIL;

		if (FAILED(Ready_Materials(pModelFilePath)))
			return E_FAIL;
	}
	else if ((!strcmp(szEXT, ".dat")))
	{
		SAVE_MODEL_DESC Data = {};
		ReadModelFile(&Data, pModelFilePath);

		if (FAILED(Ready_Meshes(&Data)))
			return E_FAIL;

		if (FAILED(Ready_Materials(&Data, pModelFilePath)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uInt iMeshIndex)
{
	m_Meshes[iMeshIndex]->Render_VIBuffer();

	return S_OK;
}

HRESULT CModel::GetMeshResource(_uInt iMeshIndex, aiTextureType eType, _uInt iTextureIndex, ID3D11ShaderResourceView** ppOut)
{
	if (m_iNumMeshes <= iMeshIndex || 0 > iMeshIndex)
		return E_FAIL;

	_uInt MatrialIndex =m_Meshes[iMeshIndex]->GetMatrialIndex();
	if (MatrialIndex >= m_iNumMaterials || MatrialIndex < 0)
		return E_FAIL;

	*ppOut = m_Materials[MatrialIndex]->GetMaterial(eType, iTextureIndex);
	if (nullptr == *ppOut)
		return E_FAIL;

	return S_OK;
}

void CModel::Export(const char* FilePath)
{
	SAVE_MODEL_DESC ExportData;

	//데이터를 여기서 구조체로 만들어서 Export한다.
	// 
	//Mesh Data Export
	ExportData.iNumMeshes = m_iNumMeshes;
	ExportData.MeshDesc.reserve(ExportData.iNumMeshes);
	for (auto iter : m_Meshes)
	{
		SAVE_MESH_DESC	MeshDesc;
		if (FAILED(iter->Export(&MeshDesc)))
			continue;
		else
			ExportData.MeshDesc.push_back(MeshDesc);
	}

	//Matrial Data Export
	ExportData.iNumMaterials = m_iNumMaterials;
	ExportData.MatrialDesc.reserve(ExportData.iNumMaterials);
	for (auto iter : m_Materials)
	{
		SAVE_MATERIAL_DESC MatrialDesc;
		if(FAILED(iter->Export(&MatrialDesc)))
			continue;
		else
			ExportData.MatrialDesc.push_back(MatrialDesc);
	}

	//여기서 직렬화
	if (FAILED(SaveModelFile(&ExportData, FilePath)))
		MSG_BOX("SAVE FAIL : MODEL DATA");
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;
	for (_uInt i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAIScene->mMeshes[i]);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;
	for (_uInt i = 0; i < m_iNumMaterials; ++i)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, pModelFilePath, m_pAIScene->mMaterials[i]);
		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
	}

	return S_OK;
}

HRESULT CModel::Ready_Meshes(void* MeshDesc)
{
	SAVE_MODEL_DESC* ExportData = static_cast<SAVE_MODEL_DESC*>(MeshDesc);
	m_iNumMeshes = ExportData->iNumMeshes;
	for (auto& Meshs : ExportData->MeshDesc)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, &Meshs);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(void* MatrialDesc, const _char* pModelFilePath)
{
	SAVE_MODEL_DESC* ExportData = static_cast<SAVE_MODEL_DESC*>(MatrialDesc);
	m_iNumMaterials = ExportData->iNumMaterials;
	for (auto& Matrials : ExportData->MatrialDesc)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, pModelFilePath, &Matrials);
		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
	}

	return S_OK;
}

HRESULT CModel::SaveModelFile(void* Data, const char* FilePath)
{
	if (nullptr == Data)
		return E_FAIL;

	SAVE_MODEL_DESC ExportData = *static_cast<SAVE_MODEL_DESC *>(Data);
	ios_base::openmode flag;
	flag = ios::out | ios::trunc | ios::binary;
	char ResoucePath[MAX_PATH] = {};

	ofstream file(FilePath, flag);
	if (file.is_open())
	{
		file.write(reinterpret_cast<char*>(&ExportData.iNumMeshes), sizeof(_uInt));
		for (auto& MeshDesc : ExportData.MeshDesc)
		{
			file.write(reinterpret_cast<char*>(&MeshDesc.iNumVertices), sizeof(_uInt));
			file.write(reinterpret_cast<char*>(&MeshDesc.iNumFaces), sizeof(_uInt));
			file.write(reinterpret_cast<char*>(&MeshDesc.iNumMaterialIndex), sizeof(_uInt));
			for (VTX_MESH& Vertex : MeshDesc.Vertices)
				file.write(reinterpret_cast<char*>(&Vertex), sizeof(VTX_MESH));

			for (_uInt& Index : MeshDesc.Indices)
				file.write(reinterpret_cast<char*>(&Index), sizeof(_uInt));
		}

		file.write(reinterpret_cast<char*>(&ExportData.iNumMaterials), sizeof(_uInt));
		for (auto& MatrialDesc : ExportData.MatrialDesc)
		{
			file.write(reinterpret_cast<char*>(&MatrialDesc.iTextureTypeMax), sizeof(_uInt));
			for (auto& TextureType : MatrialDesc.TextureType)
			{
				file.write(reinterpret_cast<char*>(&TextureType.iNumShaderResourceView), sizeof(_uInt));
				for (auto& ResourcePath : TextureType.TexturePath)
				{
					CStringHelper::ConvertWideToUTF(ResourcePath.c_str(), ResoucePath);
					file.write(ResoucePath, MAX_PATH);
				}
			}
		}
	}

	file.close();
	return S_OK;
}

HRESULT CModel::ReadModelFile(void* Data, const char* FilePath)
{
	if (nullptr == Data)
		return E_FAIL;

	SAVE_MODEL_DESC* ExportData = static_cast<SAVE_MODEL_DESC*>(Data);
	ios_base::openmode flag;
	flag = ios::in | ios::binary;
	char ResoucePath[MAX_PATH] = {};
	WCHAR WideResoucePath[MAX_PATH] = {};

	ifstream file(FilePath, flag);
	if (file.is_open())
	{
		file.read(reinterpret_cast<char*>(&ExportData->iNumMeshes), sizeof(_uInt));
		for (_uInt i = 0; i < ExportData->iNumMeshes; ++i)
		{
			SAVE_MESH_DESC MeshDesc;
			file.read(reinterpret_cast<char*>(&MeshDesc.iNumVertices), sizeof(_uInt));
			file.read(reinterpret_cast<char*>(&MeshDesc.iNumFaces), sizeof(_uInt));
			file.read(reinterpret_cast<char*>(&MeshDesc.iNumMaterialIndex), sizeof(_uInt));

			MeshDesc.Vertices.reserve(MeshDesc.iNumVertices);
			for (_uInt j = 0; j < MeshDesc.iNumVertices; ++j)
			{
				VTX_MESH Vertex;
				file.read(reinterpret_cast<char*>(&Vertex.vPosition), sizeof(_float3));
				file.read(reinterpret_cast<char*>(&Vertex.vNormal), sizeof(_float3));
				file.read(reinterpret_cast<char*>(&Vertex.vTangent), sizeof(_float3));
				file.read(reinterpret_cast<char*>(&Vertex.vTexcoord), sizeof(_float2));
				MeshDesc.Vertices.push_back(Vertex);
			}

			MeshDesc.Indices.reserve(MeshDesc.iNumFaces * 3);
			for (_uInt j = 0; j < MeshDesc.iNumFaces * 3; ++j)
			{
				_uInt Index;
				file.read(reinterpret_cast<char*>(&Index), sizeof(_uInt));
				MeshDesc.Indices.push_back(Index);
			}
			ExportData->MeshDesc.push_back(MeshDesc);
		}

		file.read(reinterpret_cast<char*>(&ExportData->iNumMaterials), sizeof(_uInt));
		for (_uInt i = 0; i < ExportData->iNumMaterials; ++i)
		{
			SAVE_MATERIAL_DESC MatrialDesc;
			file.read(reinterpret_cast<char*>(&MatrialDesc.iTextureTypeMax), sizeof(_uInt));
			for (_uInt j = 0; j < MatrialDesc.iTextureTypeMax; ++j)
			{
				MATRIAL_TEXTURE_TYPE_DESC TextureDesc;
				file.read(reinterpret_cast<char*>(&TextureDesc.iNumShaderResourceView), sizeof(_uInt));

				for (_uInt k = 0; k < TextureDesc.iNumShaderResourceView; ++k)
				{
					file.read(ResoucePath, MAX_PATH);

					CStringHelper::ConvertUTFToWide(ResoucePath, WideResoucePath);
					TextureDesc.TexturePath.push_back(WideResoucePath);
				}

				MatrialDesc.TextureType.push_back(TextureDesc);
			}

			ExportData->MatrialDesc.push_back(MatrialDesc);
		}
	}

	file.close();
	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, const _char* pModelFilePath)
{
	CModel* pModel = new CModel(pDevice, pContext);
	if (FAILED(pModel->Initialize_Prototype(eType, pModelFilePath)))
	{
		Safe_Release(pModel);
		MSG_BOX("CREATE FAIL : MODEL");
	}

	return pModel;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pModel = new CModel(*this);
	if (FAILED(pModel->Initialize(pArg)))
	{
		Safe_Release(pModel);
		MSG_BOX("CLONE FAIL : MODEL");
	}

	return pModel;
}

void CModel::Free()
{
	__super::Free();

	for (auto& iter : m_Meshes)
		Safe_Release(iter);

	for (auto& iter : m_Materials)
		Safe_Release(iter);

	m_Importer.FreeScene();
}
