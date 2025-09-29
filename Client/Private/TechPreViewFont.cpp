#include "TechPreViewFont.h"

#include "GameInstance.h"

CTechPreViewFont::CTechPreViewFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CTechPreViewFont::CTechPreViewFont(const CTechPreViewFont& rhs) :
	CBackGround(rhs)
{
}

HRESULT CTechPreViewFont::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CTechPreViewFont::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CTechPreViewFont::Update(_float fDeletaTime)
{
	_vector FontBound = m_pFontCom->GetFontBoundBox(m_szItemName.c_str());
	m_vFontPoint = { GetViewPos().x - FontBound.m128_f32[0] * 0.5f, GetViewPos().y};
}

void CTechPreViewFont::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechPreViewFont::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(1);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	m_pFontCom->Render(m_szItemName.c_str(), { 1.f,1.f,1.f,1.f });
	return S_OK;
}

void CTechPreViewFont::SetText(const WCHAR* szText)
{
	m_szItemName = szText;
}

HRESULT CTechPreViewFont::ADD_Components()
{
	// 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
	CFontComponent::FONT_DESC FontDesc = {};
	m_vFontPoint = GetViewPos();
	FontDesc.pPoint = &m_vFontPoint;
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

CTechPreViewFont* CTechPreViewFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTechPreViewFont* pPreviewFont = new CTechPreViewFont(pDevice, pContext);
	if (FAILED(pPreviewFont->Initalize_Prototype()))
	{
		Safe_Release(pPreviewFont);
		MSG_BOX("CREATE FAIL : PREVIEW FONT");
	}
	return pPreviewFont;
}

CUserInterface* CTechPreViewFont::Clone(void* pArg)
{
	CTechPreViewFont* pPreviewFont = new CTechPreViewFont(*this);
	if (FAILED(pPreviewFont->Initialize(pArg)))
	{
		Safe_Release(pPreviewFont);
		MSG_BOX("CLONE FAIL : PREVIEW FONT");
	}
	return pPreviewFont;
}

void CTechPreViewFont::Free()
{
	__super::Free();
	Safe_Release(m_pFontCom);
}
