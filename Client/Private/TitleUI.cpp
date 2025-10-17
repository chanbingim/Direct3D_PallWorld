#include "TitleUI.h"

#include "GameInstance.h"

_float4 CTitleUI::m_vBackColor = {128 / 255.f, 128/ 255.f, 128/255.f, 1.f};

CTitleUI::CTitleUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CTitleUI::CTitleUI(const CTitleUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CTitleUI::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTitleUI::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    TITLE_UI_DESC* pTitleUIDesc = static_cast<TITLE_UI_DESC*>(pArg);
    m_szTitle = pTitleUIDesc->szTitleName;

    return S_OK;
}

void CTitleUI::Update(_float fDeletaTime)
{

}

void CTitleUI::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTitleUI::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Bind_RawValue("g_vColor", &m_vBackColor, sizeof(_float4));

    m_pShaderCom->Update_Shader(4);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(m_szTitle.c_str(), { 0.f, 0.f, 0.f, 1.f });

    return S_OK;
}

void CTitleUI::SetText(const WCHAR* szTitle)
{
    m_szTitle = szTitle;
}

HRESULT CTitleUI::ADD_Components()
{
    // 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
    CFontComponent::FONT_DESC FontDesc = {};
    m_vTitleFontPos = { (_float)GetRectSize().left, (_float)GetRectSize().top };
    FontDesc.pPoint = &m_vTitleFontPos;
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

CTitleUI* CTitleUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTitleUI* pTitleUI = new CTitleUI(pDevice, pContext);
    if (FAILED(pTitleUI->Initalize_Prototype()))
    {
        Safe_Release(pTitleUI);
        MSG_BOX("CREATE FAIL : TITLE UI");
    }
    return pTitleUI;
}

CGameObject* CTitleUI::Clone(void* pArg)
{
    CTitleUI* pTitleUI = new CTitleUI(*this);
    if (FAILED(pTitleUI->Initialize(pArg)))
    {
        Safe_Release(pTitleUI);
        MSG_BOX("CLONE FAIL : TITLE UI");
    }
    return pTitleUI;
}

void CTitleUI::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
