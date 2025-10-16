#include "SmallGrass.h"

#include "GameInstance.h"

CSmallGrass::CSmallGrass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CInstanceModel(pDevice, pContext)
{
}

CSmallGrass::CSmallGrass(const CSmallGrass& rhs) :
	CInstanceModel(rhs)
{
}

HRESULT CSmallGrass::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CSmallGrass::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	RecomputeInstanceModelHeight();
	return S_OK;
}

void CSmallGrass::Priority_Update(_float fDeletaTime)
{

}

void CSmallGrass::Update(_float fDeletaTime)
{

}

void CSmallGrass::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CSmallGrass::Render()
{
	_uInt iNumMeshes = m_pVIBufferCom->GetModelIndex();

	for (_uInt i = 0; i < iNumMeshes; ++i)
	{
		__super::Apply_ConstantShaderResources(i);
		m_pShaderCom->Update_Shader(0);
		m_pVIBufferCom->Render_VIBuffer(i);
	}

	return S_OK;
}

HRESULT CSmallGrass::ADD_Components()
{
	// ¸ðµ¨ Á¤º¸
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Instance_SamllGrass"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// NonAnimShader
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_InstanceMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CSmallGrass* CSmallGrass::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSmallGrass* pSmallGrass = new CSmallGrass(pDevice, pContext);
	if (FAILED(pSmallGrass->Initalize_Prototype()))
	{
		Safe_Release(pSmallGrass);
		MSG_BOX("CREATE FAIL : SMALL GRASS");
	}

	return pSmallGrass;
}

CGameObject* CSmallGrass::Clone(void* pArg)
{
	CGameObject* pSmallGrass = new CSmallGrass(*this);
	if (FAILED(pSmallGrass->Initialize(pArg)))
	{
		Safe_Release(pSmallGrass);
		MSG_BOX("CLONE FAIL : SMALL GRASS");
	}

	return pSmallGrass;
}

void CSmallGrass::Free()
{
	__super::Free();
}