#include "Category.h"

#include "GameInstance.h"

CCategory::CCategory(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CBackGround(pGraphic_Device, pDeviceContext)
{

}

CCategory::CCategory(const CCategory& rhs) :
	CBackGround(rhs)
{
}

HRESULT CCategory::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCategory::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_eType = OBJECT_TYPE::STATIC;

	return S_OK;
}

void CCategory::Update(_float fDeletaTime)
{
}

void CCategory::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CCategory::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(2);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

void CCategory::MouseHoverEnter()
{
}

void CCategory::MouseHoverExit()
{
}

void CCategory::MouseButtonDwon()
{
}

void CCategory::SetActive(_bool flag)
{
	m_bIsActive = flag;
	if (m_bIsActive)
	{
		
	}
}

_bool CCategory::IsActive()
{
	return m_bIsActive;
}

HRESULT CCategory::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingBackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CCategory* CCategory::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
	CCategory* pCategory = new CCategory(pGraphic_Device, pDeviceContext);
	if (FAILED(pCategory->Initalize_Prototype()))
	{
		Safe_Release(pCategory);
		MSG_BOX("CREATE FAIL : CATEGORY BUTTON");
	}

	return pCategory;
}

CGameObject* CCategory::Clone(void* pArg)
{
	CCategory* pCategory = new CCategory(*this);
	if (FAILED(pCategory->Initialize(pArg)))
	{
		Safe_Release(pCategory);
		MSG_BOX("CLONE FAIL : CATEGORY BUTTON");
	}

	return pCategory;
}

void CCategory::Free()
{
	__super::Free();
}
