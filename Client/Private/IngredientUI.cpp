#include "IngredientUI.h"

#include "GameInstance.h"
#include "ItemSlotIcon.h"
#include "IngredientCountView.h"

CIngredientUI::CIngredientUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CIngredientUI::CIngredientUI(const CIngredientUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CIngredientUI::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CIngredientUI::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CIngredientUI::Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CIngredientUI::Late_Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CIngredientUI::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    if(nullptr != m_ItemData)
        m_pFontCom->Render(m_ItemData->szItemName, { 0.f , 0.f, 0.f, 1.f});
    return S_OK;
}

HRESULT CIngredientUI::ADD_Components()
{
    _float3 vScale = m_pTransformCom->GetScale();

    // 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
    CFontComponent::FONT_DESC FontDesc = {};
    m_vFontPosition = { (_float)GetRectSize().left, (_float)GetRectSize().top };
    FontDesc.pPoint = &m_vFontPosition;
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

    CItemSlotIcon::ITEM_SLOT_ICON_DESC SlotIconDesc = {};
    SlotIconDesc.pParentTransform = m_pTransformCom;
    SlotIconDesc.vScale = { vScale.y, vScale.y, 0.f};
    SlotIconDesc.vPosition = { -vScale.x * 0.5f + SlotIconDesc.vScale.x * 0.5f , 0.f ,0.f};
    m_pItemIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pItemIcon->SetZOrder(m_iZOrder + 1);
    if (FAILED(m_pItemIcon->Initialize(&SlotIconDesc)))
        return E_FAIL;

    CIngredientCountView::GAMEOBJECT_DESC IngredientCountDesc = {};
    IngredientCountDesc.pParent = this;
    IngredientCountDesc.vScale = { vScale.x * 0.3f, vScale.y, 0.f };
    IngredientCountDesc.vPosition = { vScale.x * 0.5f - IngredientCountDesc.vScale.x * 0.5f , 0.f ,0.f };
    m_pIngredientCountView = CIngredientCountView::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pIngredientCountView->SetZOrder(m_iZOrder + 1);
    if (FAILED(m_pIngredientCountView->Initialize(&IngredientCountDesc)))
        return E_FAIL;

    return S_OK;
}

CIngredientUI* CIngredientUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CIngredientUI* pIngredientUI = new CIngredientUI(pDevice, pContext);
    if (FAILED(pIngredientUI->Initalize_Prototype()))
    {
        Safe_Release(pIngredientUI);
        MSG_BOX("CREATE FAIL : INGREDIENT UI");
    }
    return pIngredientUI;
}

CGameObject* CIngredientUI::Clone(void* pArg)
{
    CIngredientUI* pIngredientUI = new CIngredientUI(*this);
    if (FAILED(pIngredientUI->Initialize(pArg)))
    {
        Safe_Release(pIngredientUI);
        MSG_BOX("CLONE FAIL : INGREDIENT UI");
    }
    return pIngredientUI;
}

void CIngredientUI::Free()
{
    __super::Free();

    Safe_Release(m_pItemIcon);
    Safe_Release(m_pFontCom);
    Safe_Release(m_pIngredientCountView);
}
