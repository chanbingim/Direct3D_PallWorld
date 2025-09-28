#include "TechSlotTitleBar.h"

#include "GameInstance.h"

CTechSlotTitleBar::CTechSlotTitleBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CTechSlotTitleBar::CTechSlotTitleBar(const CTechSlotTitleBar& rhs) :
    CBackGround(rhs)
{
}

HRESULT CTechSlotTitleBar::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTechSlotTitleBar::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CTechSlotTitleBar::Update(_float fDeletaTime)
{
    UpdateRectSize();
}

void CTechSlotTitleBar::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechSlotTitleBar::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(m_szTechTypeName.c_str(), { 0.f, 0.f, 0.f, 1.f });
    return S_OK;
}

void CTechSlotTitleBar::SetTehcItemTypeName(const WCHAR* pTechTypeName)
{
    m_szTechTypeName = pTechTypeName;

    _vector FontBound = m_pFontCom->GetFontBoundBox(pTechTypeName);
    _float halfX = FontBound.m128_f32[0] * 0.5f;

    m_fFontPoint = GetScreenPos();
    m_fFontPoint.x -= halfX;
}

HRESULT CTechSlotTitleBar::ADD_Components()
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

CTechSlotTitleBar* CTechSlotTitleBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTechSlotTitleBar* pTechnologySlotTitle = new CTechSlotTitleBar(pDevice, pContext);
    if (FAILED(pTechnologySlotTitle->Initalize_Prototype()))
    {
        Safe_Release(pTechnologySlotTitle);
        MSG_BOX("CREATE FAIL : TECHNOLOGY SLOT TITLE");
    }
    return pTechnologySlotTitle;
}

CGameObject* CTechSlotTitleBar::Clone(void* pArg)
{
    CTechSlotTitleBar* pTechnologySlotTitle = new CTechSlotTitleBar(*this);
    if (FAILED(pTechnologySlotTitle->Initialize(pArg)))
    {
        Safe_Release(pTechnologySlotTitle);
        MSG_BOX("CLONE FAIL : TECHNOLOGY SLOT TITLE");
    }
    return pTechnologySlotTitle;
}

void CTechSlotTitleBar::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
