#include "TechSlotNameBar.h"

#include "GameInstance.h"

_float4        CTechSlotNameBar::m_vColor = { 44/ 255, 44 / 255, 61 / 255, 0.9f };

CTechSlotNameBar::CTechSlotNameBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CTechSlotNameBar::CTechSlotNameBar(const CTechSlotNameBar& rhs) :
    CBackGround(rhs)
{
}

HRESULT CTechSlotNameBar::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTechSlotNameBar::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CTechSlotNameBar::Update(_float fDeletaTime)
{
    UpdateRectSize();

}

void CTechSlotNameBar::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechSlotNameBar::Render()
{
    Apply_ConstantShaderResources();

    m_pShaderCom->Bind_SRV("g_Texture", m_pTextureCom->GetTexture(0));
    m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4));

    m_pShaderCom->Update_Shader(2);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(m_szTechItemName.c_str(), {1.f, 1.f, 1.f, 1.f});
    return S_OK;
}

void CTechSlotNameBar::SetTehcItemName(const WCHAR* pTechItemName)
{
    m_szTechItemName = pTechItemName;
   
    _vector FontBound = m_pFontCom->GetFontBoundBox(pTechItemName);

    m_fFontPoint = GetScreenPos();
    m_fFontPoint.x -= FontBound.m128_f32[0] * 0.5f;
    m_fFontPoint.y -= FontBound.m128_f32[1] * 0.5f;
}

HRESULT CTechSlotNameBar::ADD_Components()
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

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Button"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CTechSlotNameBar* CTechSlotNameBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTechSlotNameBar* pTechnologySlotName = new CTechSlotNameBar(pDevice, pContext);
    if (FAILED(pTechnologySlotName->Initalize_Prototype()))
    {
        Safe_Release(pTechnologySlotName);
        MSG_BOX("CREATE FAIL : TECHNOLOGY SLOT NAME");
    }
    return pTechnologySlotName;
}

CGameObject* CTechSlotNameBar::Clone(void* pArg)
{
    CTechSlotNameBar* pTechnologySlotName = new CTechSlotNameBar(*this);
    if (FAILED(pTechnologySlotName->Initialize(pArg)))
    {
        Safe_Release(pTechnologySlotName);
        MSG_BOX("CLONE FAIL : TECHNOLOGY SLOT NAME");
    }
    return pTechnologySlotName;
}

void CTechSlotNameBar::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
