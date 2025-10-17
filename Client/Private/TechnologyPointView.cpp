#include "TechnologyPointView.h"

#include "GameInstance.h"
#include "PlayerManager.h"

CTechnologyPointView::CTechnologyPointView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext),
	m_pPlayerManager(CPlayerManager::GetInstance())
{
	Safe_AddRef(m_pPlayerManager);
}

CTechnologyPointView::CTechnologyPointView(const CTechnologyPointView& rhs) :
	CBackGround(rhs),
	m_pPlayerManager(CPlayerManager::GetInstance())
{
	Safe_AddRef(m_pPlayerManager);
}

HRESULT CTechnologyPointView::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTechnologyPointView::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_szTechPoint = L"무한대";
	return S_OK;
}

void CTechnologyPointView::Update(_float fDeletaTime)
{
	// 여기서 테크포인트 가져오자 일단 테크포인트 10 정도로 설정 그리고
	//	테스트할때는 테크포인트 의미없게 테스트
	//	m_pPlayerManager-
}

void CTechnologyPointView::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechnologyPointView::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(2);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	m_pFontCom->Render(TEXT("Technology Point"), { 1.f, 1.f, 1.f, 1.f });
	m_pPointFontCom->Render(m_szTechPoint.c_str(), {1.f, 1.f, 1.f, 1.f});

	return S_OK;
}

HRESULT CTechnologyPointView::ADD_Components()
{
	// 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
	CFontComponent::FONT_DESC FontDesc = {};
	m_fFontPoint = { (_float)GetRectSize().left, (_float)GetRectSize().top };

	FontDesc.pPoint = &m_fFontPoint;
	FontDesc.szUseFontName = TEXT("HanSanFont_16");
	FontDesc.vFontSize = { 50, 100 };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
		return E_FAIL;

	// 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
	m_fSkillFontPoint = { (_float)GetRectSize().right - 30.f,  (_float)GetRectSize().top };

	FontDesc.pPoint = &m_fSkillFontPoint;
	FontDesc.szUseFontName = TEXT("HanSanFont_16");
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("SkillPointFont_Com"), (CComponent**)&m_pPointFontCom, &FontDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Category_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CTechnologyPointView* CTechnologyPointView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTechnologyPointView* pTechnologyPointView = new CTechnologyPointView(pDevice, pContext);
	if(FAILED(pTechnologyPointView->Initalize_Prototype()))
	{
		Safe_Release(pTechnologyPointView);
		MSG_BOX("CREATE FAIL : TECHNOLOGY POINT VIEW");
	}
	return pTechnologyPointView;
}

CGameObject* CTechnologyPointView::Clone(void* pArg)
{
	CTechnologyPointView* pTechnologyPointView = new CTechnologyPointView(*this);
	if (FAILED(pTechnologyPointView->Initialize(pArg)))
	{
		Safe_Release(pTechnologyPointView);
		MSG_BOX("CLONE FAIL : TECHNOLOGY POINT VIEW");
	}
	return pTechnologyPointView;
}

void CTechnologyPointView::Free()
{
	__super::Free();

	Safe_Release(m_pFontCom);
	Safe_Release(m_pPointFontCom);
	Safe_Release(m_pPlayerManager);
}
