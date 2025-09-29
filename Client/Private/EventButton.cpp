#include "EventButton.h"

#include "GameInstance.h"

CEventButton::CEventButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CEventButton::CEventButton(const CEventButton& rhs) :
    CBackGround(rhs)
{
}

HRESULT CEventButton::Initalize_Prototype()
{
    if(FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEventButton::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_bIsMouseEvent = true;
    return S_OK;
}

void CEventButton::Update(_float fDeletaTime)
{
    UpdateRectSize();
    m_fFontPoint = { (_float)GetRectSize().left, (_float)GetRectSize().top };
    __super::Update(fDeletaTime);
}

void CEventButton::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CEventButton::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();
   

    m_pFontCom->Render(m_szViewName.c_str(), { 1.f,1.f,1.f,1.f });

    return S_OK;
}

void CEventButton::Set_Text(const WCHAR* szText)
{
    m_szViewName = szText;
}

void CEventButton::Bind_Event(function<void()> Event)
{
    m_ClickEvent = Event;
}

void CEventButton::MouseHoverEnter()
{
}

void CEventButton::MouseHovering()
{
}

void CEventButton::MouseHoverExit()
{
}

void CEventButton::MouseButtonDwon()
{
}

void CEventButton::MouseButtonPressed()
{
}

void CEventButton::MouseButtonUp()
{
    if (m_bIsHover)
    {
        if (nullptr != m_ClickEvent)
            m_ClickEvent();
    }
}

HRESULT CEventButton::ADD_Components()
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

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Button_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CEventButton* CEventButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEventButton* pEventButton = new CEventButton(pDevice, pContext);
    if (FAILED(pEventButton->Initalize_Prototype()))
    {
        Safe_Release(pEventButton);
        MSG_BOX("CREATE FAIL : EVENT BUTTON");
    }
    return pEventButton;
}

CGameObject* CEventButton::Clone(void* pArg)
{
    CEventButton* pEventButton = new CEventButton(*this);
    if (FAILED(pEventButton->Initialize(pArg)))
    {
        Safe_Release(pEventButton);
        MSG_BOX("CLONE FAIL : EVENT BUTTON");
    }
    return pEventButton;
}

void CEventButton::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
