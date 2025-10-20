#ifdef _DEBUG
#include "ImgManager.h"
#include "IMG_LandScape.h"
#endif // _DEBUG

#include "BossMap.h"
#include "TerrainManager.h"

#include "GameInstance.h"

CBossMap::CBossMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CNoneAnimMesh(pDevice, pContext)
{
}

CBossMap::CBossMap(const CBossMap& rhs) :
	CNoneAnimMesh(rhs)
{
}

HRESULT CBossMap::Initalize_Prototype()
{

	return S_OK;
}

HRESULT CBossMap::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_ObejctTag = TEXT("MapObject");
	return S_OK;
}

void CBossMap::Priority_Update(_float fDeletaTime)
{
}

void CBossMap::Update(_float fDeletaTime)
{
}

void CBossMap::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBossMap::Render()
{
	_uInt iNum = m_pVIBufferCom->GetNumMeshes();
	for (_uInt i = 0; i < iNum; ++i)
	{
		Apply_ConstantShaderResources(i);

		m_pShaderCom->Update_Shader(0);
		m_pVIBufferCom->Render(i);
	}
	CTerrainManager::GetInstance()->Render();

	return S_OK;
}

HRESULT CBossMap::Bind_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
	m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
	m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
	m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();

	return S_OK;
}

HRESULT CBossMap::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

	ID3D11ShaderResourceView* pResourceVeiw = {};
	m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
	if (pResourceVeiw)
		m_pSRVEffect->SetResource(pResourceVeiw);

	ID3D11ShaderResourceView* pNormalRSV = {};
	m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_NORMALS, 0, &pNormalRSV);
	if (pNormalRSV)
		m_pShaderCom->Bind_SRV("g_NormalTexture", pNormalRSV);

	return S_OK;
}

HRESULT CBossMap::ADD_Components()
{
	// µ¹  ¸ðµ¨ Á¤º¸
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_BossMap"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// NonAnimShader
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CBossMap* CBossMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBossMap* pBossMap = new CBossMap(pDevice, pContext);
	if (FAILED(pBossMap->Initalize_Prototype()))
	{
		Safe_Release(pBossMap);
		MSG_BOX("CREATE FAIL : BOSS MAP");
	}
	return pBossMap;
}

CGameObject* CBossMap::Clone(void* pArg)
{
	CBossMap* pBossMap = new CBossMap(*this);
	if (FAILED(pBossMap->Initialize(pArg)))
	{
		Safe_Release(pBossMap);
		MSG_BOX("CLONE FAIL : BOSS MAP");
	}
	return pBossMap;
}

void CBossMap::Free()
{
	__super::Free();
}
