#include "FastTravelButton.h"

#include "GameInstance.h"

CFastTravelButton::CFastTravelButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CButton(pDevice, pContext)
{
}

CFastTravelButton::CFastTravelButton(const CFastTravelButton& rhs) :
    CButton(rhs)
{
}

HRESULT CFastTravelButton::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CFastTravelButton::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    FAST_TRAVEL_DESC* pTravelDesc = static_cast<FAST_TRAVEL_DESC*>(pArg);
    m_szMoveMap = pTravelDesc->szMapName;
    m_szFontText = pTravelDesc->szShowFontText;

    _vector vFontSize = m_pFontCom->GetFontBoundBox(m_szFontText.c_str());
    m_vFontPos = { GetScreenPos().x - vFontSize.m128_f32[0] * 0.5f, GetScreenPos().y - vFontSize.m128_f32[1] * 0.5f };

    return S_OK;
}

void CFastTravelButton::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CFastTravelButton::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CFastTravelButton::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(m_szFontText.c_str(), {0.f, 0.f, 0.f, 1.f});
    return S_OK;
}

void CFastTravelButton::Bind_ClickEvent(function<void(const _wstring&)> ClickFunc)
{
    m_ClickEventFunc = ClickFunc;
}

void CFastTravelButton::MouseButtonDwon()
{
    if (m_ClickEventFunc)
        m_ClickEventFunc(m_szMoveMap);
}

HRESULT CFastTravelButton::ADD_Components()
{
    CFontComponent::FONT_DESC FontDesc = {};

    FontDesc.pPoint = &m_vFontPos;
    FontDesc.vFontSize = {};
    FontDesc.szUseFontName = TEXT("HanSanFont_16");
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_Category_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CFastTravelButton* CFastTravelButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFastTravelButton* pFastTravelButton = new CFastTravelButton(pDevice, pContext);
    if (FAILED(pFastTravelButton->Initalize_Prototype()))
    {
        Safe_Release(pFastTravelButton);
        MSG_BOX("CREATE FAIL : FAST TRAVEL BUTTON");
    }
    return pFastTravelButton;
}

CGameObject* CFastTravelButton::Clone(void* pArg)
{
    CFastTravelButton* pFastTravelButton = new CFastTravelButton(*this);
    if (FAILED(pFastTravelButton->Initialize(pArg)))
    {
        Safe_Release(pFastTravelButton);
        MSG_BOX("CLONE FAIL : FAST TRAVEL BUTTON");
    }
    return pFastTravelButton;
}

void CFastTravelButton::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
