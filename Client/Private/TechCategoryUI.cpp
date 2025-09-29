#include "TechCategoryUI.h"

#include "GameInstance.h"
#include "TechManager.h"
#include "TechTabIcon.h"

CTechCategoryUI::CTechCategoryUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CTechCategoryUI::CTechCategoryUI(const CTechCategoryUI& rhs) :
	CBackGround(rhs)
{
}

HRESULT CTechCategoryUI::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTechCategoryUI::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	TECH_CATEGORY_DESC* pCategoryDesc = static_cast<TECH_CATEGORY_DESC*>(pArg);
	m_eTechButtonType = pCategoryDesc->CategoryType;
	m_pTechTabIcon->SetTexIcon(ENUM_CLASS(pCategoryDesc->CategoryType));

	m_bIsMouseEvent = true;
	return S_OK;
}

void CTechCategoryUI::Update(_float fDeletaTime)
{
	__super::Update(fDeletaTime);
	m_pTechTabIcon->Update(fDeletaTime);

	m_pTpyeName = CTechManager::GetInstance()->GetTechTypeToString(m_eTechButtonType);
	_vector size = m_pFontCom->GetFontBoundBox(m_pTpyeName.c_str());

	m_fFontPoint = { GetScreenPos().x - size.m128_f32[0] * 0.5f,  GetScreenPos().y + 20.f};
}

void CTechCategoryUI::Late_Update(_float fDeletaTime)
{
	m_pTechTabIcon->Late_Update(fDeletaTime);
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechCategoryUI::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(2);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	m_pFontCom->Render(m_pTpyeName.c_str(), {1.f,1.f,1.f,1.f});
	return S_OK;
}

void CTechCategoryUI::Bind_EventFunction(function<void(_uInt)> ClieckEvent)
{
	m_ClickEvent = ClieckEvent;
}

void CTechCategoryUI::MouseHoverEnter()
{

}

void CTechCategoryUI::MouseHoverExit()
{

}

void CTechCategoryUI::MouseButtonDwon()
{
	m_ClickEvent(ENUM_CLASS(m_eTechButtonType));
}

HRESULT CTechCategoryUI::ADD_Components()
{
	// 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
	CFontComponent::FONT_DESC FontDesc = {};
	m_fFontPoint = GetScreenPos();
	

	FontDesc.pPoint = &m_fFontPoint;
	FontDesc.szUseFontName = TEXT("HanSanFont_16");
	FontDesc.vFontSize = { 50, 100 };

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Category_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	CTechTabIcon::GAMEOBJECT_DESC TechIconDesc = { };
	TechIconDesc.pParent = this;
	TechIconDesc.vScale = { 100.f, 45.f, 0.f };
	TechIconDesc.vPosition = { 0.f, -TechIconDesc.vScale.y * 0.40f, 0.f};
	m_pTechTabIcon = CTechTabIcon::Create(m_pGraphic_Device, m_pDeviceContext);
	m_pTechTabIcon->SetZOrder(m_iZOrder + 1);
	if (FAILED(m_pTechTabIcon->Initialize(&TechIconDesc)))
		return E_FAIL;

	return S_OK;
}

CTechCategoryUI* CTechCategoryUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTechCategoryUI* pTechCategory = new CTechCategoryUI(pDevice, pContext);
	if (FAILED(pTechCategory->Initalize_Prototype()))
	{
		Safe_Release(pTechCategory);
		MSG_BOX("CREATE FAIL : TECH CATEGORY");
	}
	return pTechCategory;
}

CGameObject* CTechCategoryUI::Clone(void* pArg)
{
	CTechCategoryUI* pTechCategory = new CTechCategoryUI(*this);
	if (FAILED(pTechCategory->Initialize(pArg)))
	{
		Safe_Release(pTechCategory);
		MSG_BOX("CLONE FAIL : TECH CATEGORY");
	}
	return pTechCategory;
}

void CTechCategoryUI::Free()
{
	__super::Free();

	Safe_Release(m_pTechTabIcon);
	Safe_Release(m_pFontCom);
}
