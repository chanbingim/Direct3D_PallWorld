#include "ItemQuickSlotName.h"

#include "GameInstance.h"

CItemQuickSlotName::CItemQuickSlotName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CItemQuickSlotName::CItemQuickSlotName(const CItemQuickSlotName& rhs) :
    CBackGround(rhs)
{
}

HRESULT CItemQuickSlotName::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemQuickSlotName::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_fFontPoint = { (_float)GetRectSize().left, (_float)GetRectSize().top };
    return S_OK;
}

void CItemQuickSlotName::Update(_float fDeletaTime)
{

}

void CItemQuickSlotName::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CItemQuickSlotName::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(m_szItemName.c_str(), { 0.f, 0.f, 0.f, 1.f });
    return S_OK;
}

void CItemQuickSlotName::SetItemName(const WCHAR* szName)
{
    m_szItemName = szName;
}

HRESULT CItemQuickSlotName::ADD_Components()
{
    // 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
    CFontComponent::FONT_DESC FontDesc = {};
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

CItemQuickSlotName* CItemQuickSlotName::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItemQuickSlotName* pQuickSlot = new CItemQuickSlotName(pDevice, pContext);
    if (FAILED(pQuickSlot->Initalize_Prototype()))
    {
        Safe_Release(pQuickSlot);
        MSG_BOX("CREATE FAIL : ITEM QUICK SLOT NAME");
    }
    return pQuickSlot;
}

CGameObject* CItemQuickSlotName::Clone(void* pArg)
{
    CItemQuickSlotName* pQuickSlot = new CItemQuickSlotName(*this);
    if (FAILED(pQuickSlot->Initialize(pArg)))
    {
        Safe_Release(pQuickSlot);
        MSG_BOX("CLONE FAIL : ITEM QUICK SLOT NAME");
    }
    return pQuickSlot;
}

void CItemQuickSlotName::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
