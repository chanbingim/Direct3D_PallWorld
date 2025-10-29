#include "TechnologyTitleBar.h"

#include "GameInstance.h"

CTechnologyTitleBar::CTechnologyTitleBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CTechnologyTitleBar::CTechnologyTitleBar(const CTechnologyTitleBar& rhs) :
    CBackGround(rhs)
{
}

HRESULT CTechnologyTitleBar::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    m_vColor = { 0.6f, 0.6f, 0.6f, 0.6f };
    return S_OK;
}

HRESULT CTechnologyTitleBar::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_vColor = { 0.6f, 0.6f, 0.6f, 0.3f };
    return S_OK;
}

void CTechnologyTitleBar::Update(_float fDeletaTime)
{
}

void CTechnologyTitleBar::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechnologyTitleBar::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4));

    m_pShaderCom->Update_Shader(4);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(TEXT("Technology"), {1.f, 1.f, 1.f, 1.f});
    return S_OK;
}

HRESULT CTechnologyTitleBar::ADD_Components()
{
    // 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
    CFontComponent::FONT_DESC FontDesc = {};
    m_fFontPoint = { (_float)GetRectSize().left, (_float)GetRectSize().top };
    FontDesc.pPoint = &m_fFontPoint;
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

CTechnologyTitleBar* CTechnologyTitleBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTechnologyTitleBar* pTechnologyTitleBar = new CTechnologyTitleBar(pDevice, pContext);
    if (FAILED(pTechnologyTitleBar->Initalize_Prototype()))
    {
        Safe_Release(pTechnologyTitleBar);
        MSG_BOX("CREATE FAIL : TECHNOLOGY TITTLE BAR");
    }
    return pTechnologyTitleBar;
}

CGameObject* CTechnologyTitleBar::Clone(void* pArg)
{
    CTechnologyTitleBar* pTechnologyTitleBar = new CTechnologyTitleBar(*this);
    if (FAILED(pTechnologyTitleBar->Initialize(pArg)))
    {
        Safe_Release(pTechnologyTitleBar);
        MSG_BOX("CLONE FAIL : TECHNOLOGY TITTLE BAR");
    }
    return pTechnologyTitleBar;
}

void CTechnologyTitleBar::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}