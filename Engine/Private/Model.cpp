#include "Model.h"

#include "GameInstance.h"
#include "Mesh.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel& rhs) :
	CComponent(rhs),
	m_Meshes(rhs.m_Meshes),
	m_iNumMeshes(rhs.m_iNumMeshes)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);
}

HRESULT CModel::Initialize_Prototype(const _char* pModelFilePath)
{
	_uInt iFlags = {};

	iFlags = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlags);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render()
{
	for (auto iter : m_Meshes)
		iter->Render_VIBuffer();

	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;
	for (_uInt i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_pAIScene->mMeshes[i]);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath)
{
	CModel* pModel = new CModel(pDevice, pContext);
	if (FAILED(pModel->Initialize_Prototype(pModelFilePath)))
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

	m_Importer.FreeScene();
}
