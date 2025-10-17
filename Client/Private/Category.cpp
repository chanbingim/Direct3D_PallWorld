#include "Category.h"

#include "GameInstance.h"
#include "SelectTriangle.h"
#include "ButtonFlare.h"

CCategory::CCategory(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CBackGround(pGraphic_Device, pDeviceContext)
{

}

CCategory::CCategory(const CCategory& rhs) :
	CBackGround(rhs),
	m_fAlpha(rhs.m_fAlpha)
{
}

HRESULT CCategory::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	m_fAlpha = 0.8f;
	return S_OK;
}

HRESULT CCategory::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;

	CATEGROY_DESC* Desc = static_cast<CATEGROY_DESC*>(pArg);
	m_ButtonType = Desc->Type;
	m_FontText = Desc->szButtonName;

	_vector vFontSize = m_pFontCom->GetFontBoundBox(m_FontText.c_str());
	m_vFontPoint = { GetScreenPos().x - vFontSize.m128_f32[0] * 0.5f, GetScreenPos().y - vFontSize.m128_f32[1] * 0.5f };

	m_StartU = 0.2f;
	m_eType = OBJECT_TYPE::STATIC;
	m_bIsMouseEvent = true;

	return S_OK;
}

void CCategory::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);

}

void CCategory::Late_Update(_float fDeletaTime)
{
	if (m_bIsActive || m_bIsHover)
		m_pButtonFlare->Late_Update(fDeletaTime);

	if (m_bIsActive)
		m_pSelectTri->Late_Update(fDeletaTime);

	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CCategory::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(0);
	m_pVIBufferCom->Render_VIBuffer();
	
	m_pFontCom->Render(m_FontText.c_str(), {1.f, 1.f, 1.f, 1.f});
	return S_OK;
}

HRESULT CCategory::Apply_ConstantShaderResources()
{
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->GetWorldMat());
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
	
	m_pShaderCom->Bind_SRV("g_Texture", m_pTextureCom->GetTexture(0));
	m_pShaderCom->Bind_SRV("g_EftTexture", m_pTextureCom->GetTexture(1));

	m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float));
	m_pShaderCom->Bind_RawValue("g_fStartU", &m_StartU, sizeof(_float));
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
	if (m_ClickedFunc)
		m_ClickedFunc(m_ButtonType);
}

void CCategory::SetActive(_bool flag)
{
	m_bIsActive = flag;
}

_bool CCategory::IsActive()
{
	return m_bIsActive;
}

void CCategory::Bind_ClickEvent(function<void(_uInt Index)> Func)
{
	m_ClickedFunc = Func;
}

HRESULT CCategory::ADD_Components()
{
	CFontComponent::FONT_DESC FontDesc = {};
	FontDesc.pPoint = &m_vFontPoint;
	FontDesc.vFontSize = {};
	FontDesc.szUseFontName = TEXT("HanSanFont_16");
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_Category_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Button"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCategory::ADD_Childs()
{
	_float3 vParentScale = m_pTransformCom->GetScale();

	/* Select Triangle */
	CSelectTriangle::GAMEOBJECT_DESC SelectTriDesc = {};
	SelectTriDesc.pParent = this;
	SelectTriDesc.vScale = { 43.f , vParentScale.y, 0.f };
	SelectTriDesc.vPosition = { vParentScale.x * 0.5f, 0.f, 0.f };

	m_pSelectTri = CSelectTriangle::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(m_pSelectTri->Initialize(&SelectTriDesc)))
		return E_FAIL;

	/* Button Flare */
	CButtonFlare::GAMEOBJECT_DESC ButtonFlareDesc = {};
	ButtonFlareDesc.pParent = this;
	ButtonFlareDesc.vScale = { vParentScale.x + 30.f, vParentScale.y + 100.f, 0.f };
	ButtonFlareDesc.vPosition = { 0.f, -(ButtonFlareDesc.vScale.y * 0.5f) + vParentScale.y, 0.f};

	m_pButtonFlare = CButtonFlare::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(m_pButtonFlare->Initialize(&ButtonFlareDesc)))
		return E_FAIL;
	m_pButtonFlare->SetZOrder(m_iZOrder - 1);

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

	Safe_Release(m_pFontCom);
	Safe_Release(m_pButtonFlare);
	Safe_Release(m_pSelectTri);
}
