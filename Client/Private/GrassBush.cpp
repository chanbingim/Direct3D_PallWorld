#include "GrassBush.h"

#include "GameInstance.h"

CGrassBush::CGrassBush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CInstanceModel(pDevice, pContext)
{
}

CGrassBush::CGrassBush(const CGrassBush& rhs) :
    CInstanceModel(rhs)
{
}

HRESULT CGrassBush::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CGrassBush::Initialize(void* pArg)
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

void CGrassBush::Priority_Update(_float fDeletaTime)
{
}

void CGrassBush::Update(_float fDeletaTime)
{
}

void CGrassBush::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CGrassBush::Render()
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

HRESULT CGrassBush::ADD_Components()
{
	// ¸ðµ¨ Á¤º¸
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Instance_GrassBush"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// NonAnimShader
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_InstanceMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CGrassBush* CGrassBush::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGrassBush* pGrassBush = new CGrassBush(pDevice, pContext);
	if (FAILED(pGrassBush->Initalize_Prototype()))
	{
		Safe_Release(pGrassBush);
		MSG_BOX("CREATE FAIL : GRASS BUSH");
	}

	return pGrassBush;
}

CGameObject* CGrassBush::Clone(void* pArg)
{
	CGrassBush* pGrassBush = new CGrassBush(*this);
	if (FAILED(pGrassBush->Initialize(pArg)))
	{
		Safe_Release(pGrassBush);
		MSG_BOX("CLONE FAIL : GRASS BUSH");
	}

	return pGrassBush;
}

void CGrassBush::Free()
{
	__super::Free();
}
