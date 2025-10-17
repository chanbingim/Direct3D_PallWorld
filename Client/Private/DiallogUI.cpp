#include "DiallogUI.h"

#include "GameInstance.h"

#include "GamePlayHUD.h"

CDiallogUI::CDiallogUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CDiallogUI::CDiallogUI(const CDiallogUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CDiallogUI::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDiallogUI::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CDiallogUI::Update(_float fDeletaTime)
{
    if (VISIBILITY::HIDDEN == m_eVisible)
        return;

    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_RETURN))
    {
        CGamePlayHUD* pGamePlayHUD = static_cast<CGamePlayHUD*>(m_pGameInstance->GetCurrentHUD());
        pGamePlayHUD->UnActivePopUpUserInterface(3);
    }

    if (m_iTextLength < m_szTotalText.length())
    {
        m_szText = m_szTotalText.substr(0, m_iTextLength);
        m_iTextLength++;
    }
    else
        m_szText = m_szTotalText;
}

void CDiallogUI::Late_Update(_float fDeletaTime)
{
    if (VISIBILITY::HIDDEN == m_eVisible)
        return;

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CDiallogUI::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(m_szText.c_str(), {0.f, 0.f, 0.f ,1.f});

    return S_OK;
}

void CDiallogUI::SetDiallogText(const WCHAR* szDiallogText)
{
    m_szTotalText = szDiallogText;
    m_iTextLength = 0;
}

HRESULT CDiallogUI::ADD_Components()
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

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Slot_Base_Texture"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CDiallogUI* CDiallogUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDiallogUI* pDiallog = new CDiallogUI(pDevice, pContext);
    if (FAILED(pDiallog->Initalize_Prototype()))
    {
        Safe_Release(pDiallog);
        MSG_BOX("CREATE FAIL : DIALLOG");
    }
    return pDiallog;
}

CGameObject* CDiallogUI::Clone(void* pArg)
{
    CDiallogUI* pDiallog = new CDiallogUI(*this);
    if (FAILED(pDiallog->Initialize(pArg)))
    {
        Safe_Release(pDiallog);
        MSG_BOX("CLONE FAIL : DIALLOG");
    }
    return pDiallog;
}

void CDiallogUI::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
