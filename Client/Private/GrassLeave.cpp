#include "GrassLeave.h"

#include "GameInstance.h"

CGrassLeave::CGrassLeave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CInstanceModel(pDevice, pContext)
{
}

CGrassLeave::CGrassLeave(const CGrassLeave& rhs) :
    CInstanceModel(rhs)
{
}

HRESULT CGrassLeave::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CGrassLeave::Initialize(void* pArg)
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

void CGrassLeave::Priority_Update(_float fDeletaTime)
{
}

void CGrassLeave::Update(_float fDeletaTime)
{
}

void CGrassLeave::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CGrassLeave::Render()
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

HRESULT CGrassLeave::ADD_Components()
{
	// ¸ðµ¨ Á¤º¸
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Instance_GrasLeave"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// NonAnimShader
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_InstanceMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CGrassLeave* CGrassLeave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGrassLeave* pGrassLeave = new CGrassLeave(pDevice, pContext);
	if (FAILED(pGrassLeave->Initalize_Prototype()))
	{
		Safe_Release(pGrassLeave);
		MSG_BOX("CREATE FAIL : GRASS LEAVE");
	}

	return pGrassLeave;
}

CGameObject* CGrassLeave::Clone(void* pArg)
{
	CGrassLeave* pGrassLeave = new CGrassLeave(*this);
	if (FAILED(pGrassLeave->Initialize(pArg)))
	{
		Safe_Release(pGrassLeave);
		MSG_BOX("CLONE FAIL : GRASS LEAVE");
	}

	return pGrassLeave;
}

void CGrassLeave::Free()
{
	__super::Free();
}
