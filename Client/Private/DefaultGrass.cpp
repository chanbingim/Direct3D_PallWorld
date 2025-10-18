#include "DefaultGrass.h"

#include "GameInstance.h"
#include "TerrainManager.h"

CDefaultGrass::CDefaultGrass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CInstanceModel(pDevice, pContext)
{
}

CDefaultGrass::CDefaultGrass(const CDefaultGrass& rhs) :
    CInstanceModel(rhs)
{
}

HRESULT CDefaultGrass::Initalize_Prototype()
{

    return S_OK;
}

HRESULT CDefaultGrass::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	RefreshComputeHeight();
	return S_OK;
}

void CDefaultGrass::Priority_Update(_float fDeletaTime)
{
}

void CDefaultGrass::Update(_float fDeletaTime)
{
}

void CDefaultGrass::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CDefaultGrass::Render()
{
	_uInt iNumMeshes = m_pVIBufferCom->GetModelIndex();

	for (_uInt i = 0; i < iNumMeshes; ++i)
	{
		__super::Apply_ConstantShaderResources(i);
		m_pShaderCom->Update_Shader(0);
		m_pVIBufferCom->Render_VIBuffer(i);
	}

	m_pBoundBox->Render();
	return S_OK;
}

HRESULT CDefaultGrass::ADD_Components()
{
	// ¸ðµ¨ Á¤º¸
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Instance_DefaultGrass"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	CBoxCollision::BOX_COLLISION_DESC BoxColDesc = {};
	BoxColDesc.pOwner = this;
	BoxColDesc.Extents = m_pVIBufferCom->GetInstanceModelBoundSize();

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionBox"), TEXT("Collision_Com"), (CComponent**)&m_pBoundBox, &BoxColDesc)))
		return E_FAIL;

	// NonAnimShader
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_InstanceMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CDefaultGrass* CDefaultGrass::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDefaultGrass* pDefaultGrass = new CDefaultGrass(pDevice, pContext);
	if (FAILED(pDefaultGrass->Initalize_Prototype()))
	{
		Safe_Release(pDefaultGrass);
		MSG_BOX("CREATE FAIL : SMALL GRASS");
	}

	return pDefaultGrass;
}

CGameObject* CDefaultGrass::Clone(void* pArg)
{
	CDefaultGrass* pDefaultGrass = new CDefaultGrass(*this);
	if (FAILED(pDefaultGrass->Initialize(pArg)))
	{
		Safe_Release(pDefaultGrass);
		MSG_BOX("CLONE FAIL : SMALL GRASS");
	}

	return pDefaultGrass;
}

void CDefaultGrass::Free()
{
	__super::Free();
}
