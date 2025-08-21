#include "Frog.h"

#include "GameInstance.h"

CFrog::CFrog(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CPellBase(pGraphic_Device, pDeviceContext)
{
}

CFrog::CFrog(const CFrog& rhs) :
	CPellBase(rhs)
{
}

HRESULT CFrog::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFrog::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CFrog::Priority_Update(_float fDeletaTime)
{
}

void CFrog::Update(_float fDeletaTime)
{
}

void CFrog::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CFrog::Render()
{
	_uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

	for (_uInt i = 0; i < iNumMeshes; ++i)
	{
		Apply_ConstantShaderResources(i);

		m_pShaderCom->Update_Shader(0);

		m_pVIBufferCom->Render(i);
	}

	return S_OK;
}

HRESULT CFrog::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Frog_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CFrog* CFrog::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
	CFrog* pFrog = new CFrog(pGraphic_Device, pDeviceContext);
	if (FAILED(pFrog->Initalize_Prototype()))
	{
		Safe_Release(pFrog);
		MSG_BOX("CREATE FAIL : Frog");
	}
	return pFrog;
}

CGameObject* CFrog::Clone(void* pArg)
{
	CFrog* pFrog = new CFrog(*this);
	if (FAILED(pFrog->Initialize(pArg)))
	{
		Safe_Release(pFrog);
		MSG_BOX("CLONE FAIL : Frog");
	}
	return pFrog;
}

void CFrog::Free()
{
	__super::Free();
}
