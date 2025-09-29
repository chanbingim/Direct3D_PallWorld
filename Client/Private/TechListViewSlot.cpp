#include "TechListViewSlot.h"

#include "GameInstance.h"
#include "ItemSlotIcon.h"

#include "ItemManager.h"

CTechListViewSlot::CTechListViewSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{
}

CTechListViewSlot::CTechListViewSlot(const CTechListViewSlot& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CTechListViewSlot::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTechListViewSlot::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CTechListViewSlot::Update(_float fDeletaTime)
{
    m_pItemIcon->Update(fDeletaTime);

    _vector vFontBound = m_pFontCom->GetFontBoundBox(m_szItemName.c_str());
    m_fFontPoint = { GetScreenPos().x - vFontBound.m128_f32[0],
                     (_float)GetRectSize().bottom + m_fFontPoint .y * 2.f};
}

void CTechListViewSlot::Late_Update(_float fDeletaTime)
{
    m_pItemIcon->Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechListViewSlot::Render()
{
    m_pFontCom->Render(m_szItemName.c_str(), { 1.f, 1.f, 1.f, 1.f });

    return S_OK;
}

void CTechListViewSlot::SetViewItemInfo(const ITEM_DESC* pItemDesc)
{
    m_pItemIcon->SetTexture(CItemManager::GetInstance()->GetItemTexture(pItemDesc->iItemNum));
    m_szItemName = pItemDesc->szItemName;
}

HRESULT CTechListViewSlot::ADD_Components()
{
    // 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
    CFontComponent::FONT_DESC FontDesc = {};
    m_fFontPoint = GetViewPos();
    FontDesc.pPoint = &m_fFontPoint;
    FontDesc.szUseFontName = TEXT("HanSanFont_16");
    FontDesc.vFontSize = { 50, 100 };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
        return E_FAIL;

    _float3 vScale = m_pTransformCom->GetScale();
    CItemSlotIcon::ITEM_SLOT_ICON_DESC ItemSlotDesc = {};
    ItemSlotDesc.pParent = this;

    ItemSlotDesc.vScale = { vScale.x * 0.7f, vScale.y * 0.7f, 0.f };
    ItemSlotDesc.vPosition = {0.f,  ItemSlotDesc.vScale.y * 0.5f, 0.f };
    m_pItemIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(m_pItemIcon->Initialize(&ItemSlotDesc)))
        return E_FAIL;

    return S_OK;
}

CTechListViewSlot* CTechListViewSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTechListViewSlot* pTechListView = new CTechListViewSlot(pDevice, pContext);
    if (FAILED(pTechListView->Initalize_Prototype()))
    {
        Safe_Release(pTechListView);
        MSG_BOX("CREATE FAIL : TECH LIST VIEW SLOT");
    }
    return pTechListView;
}

CGameObject* CTechListViewSlot::Clone(void* pArg)
{
    CTechListViewSlot* pTechListView = new CTechListViewSlot(*this);
    if (FAILED(pTechListView->Initialize(pArg)))
    {
        Safe_Release(pTechListView);
        MSG_BOX("CLONE FAIL : TECH LIST VIEW SLOT");
    }
    return pTechListView;
}

void CTechListViewSlot::Free()
{
    __super::Free();

    Safe_Release(m_pItemIcon);
    Safe_Release(m_pFontCom);
}
