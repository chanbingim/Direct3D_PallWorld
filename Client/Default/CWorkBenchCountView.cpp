#include "CWorkBenchCountView.h"

#include "GameInstance.h"

CWorkBenchCountView::CWorkBenchCountView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CWorkBenchCountView::CWorkBenchCountView(const CWorkBenchCountView& rhs) :
	CBackGround(rhs)
{
}

HRESULT CWorkBenchCountView::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWorkBenchCountView::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CWorkBenchCountView::Update(_float fDeletaTime)
{

}

void CWorkBenchCountView::Late_Update(_float fDeletaTime)
{

}

HRESULT CWorkBenchCountView::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(0);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CWorkBenchCountView::ADD_Components()
{
	// 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
	CFontComponent::FONT_DESC FontDesc = {};
	m_vFontPos = { (_float)GetRectSize().left, (_float)GetRectSize().top };
	FontDesc.pPoint = &m_vFontPos;
	FontDesc.szUseFontName = TEXT("HanSanFont_16");
	FontDesc.vFontSize = { 50, 100 };

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellInfo_Background"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CWorkBenchCountView* CWorkBenchCountView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorkBenchCountView* pWorkBenchCountView = new CWorkBenchCountView(pDevice, pContext);
	if (FAILED(pWorkBenchCountView->Initalize_Prototype()))
	{
		Safe_Release(pWorkBenchCountView);
		MSG_BOX("CREATE FAIL : WORK BENCH CREATE VIEW");
	}
	return pWorkBenchCountView;
}

CGameObject* CWorkBenchCountView::Clone(void* pArg)
{
	CWorkBenchCountView* pWorkBenchCountView = new CWorkBenchCountView(*this);
	if (FAILED(pWorkBenchCountView->Initialize(pArg)))
	{
		Safe_Release(pWorkBenchCountView);
		MSG_BOX("CLONE FAIL : WORK BENCH CREATE VIEW");
	}
	return pWorkBenchCountView;
}

void CWorkBenchCountView::Free()
{
	__super::Free();

	Safe_Release(m_pFontCom);
}
