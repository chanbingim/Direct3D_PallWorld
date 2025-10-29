#include "LevelIcon.h"

#include "GameInstance.h"

CLevelIcon::CLevelIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CLevelIcon::CLevelIcon(const CLevelIcon& rhs) :
    CBackGround(rhs)
{
}

HRESULT CLevelIcon::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    m_vColor = { 0.6f, 0.6f, 0.6f, 0.6f };
    return S_OK;
}

HRESULT CLevelIcon::Initialize(void* pArg)
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

void CLevelIcon::Update(_float fDeletaTime)
{
    UpdateRectSize();
}

void CLevelIcon::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CLevelIcon::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4));

    m_pShaderCom->Update_Shader(4);
    m_pTextureCom->SetTexture(0,0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(m_szLevel.c_str(), { 0.f, 0.f, 0.f, 1.f });
    return S_OK;
}

void CLevelIcon::SetLevel(const WCHAR* szLevel)
{
    m_szLevel = szLevel;
}

HRESULT CLevelIcon::ADD_Components()
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

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellInfo_Background"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CLevelIcon* CLevelIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLevelIcon* pLevelICon = new CLevelIcon(pDevice, pContext);
    if (FAILED(pLevelICon->Initalize_Prototype()))
    {
        Safe_Release(pLevelICon);
        MSG_BOX("CREATE FAIL : LEVEL ICON");
    }
    return pLevelICon;
}

CGameObject* CLevelIcon::Clone(void* pArg)
{
    CLevelIcon* pLevelICon = new CLevelIcon(*this);
    if (FAILED(pLevelICon->Initialize(pArg)))
    {
        Safe_Release(pLevelICon);
        MSG_BOX("CLONE FAIL : LEVEL ICON");
    }
    return pLevelICon;
}

void CLevelIcon::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
