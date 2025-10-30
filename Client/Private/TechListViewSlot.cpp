#include "TechListViewSlot.h"

#include "GameInstance.h"
#include "ItemSlotIcon.h"
#include "TechListSlotFont.h"

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

    m_bIsMouseEvent = true;
    return S_OK;
}

void CTechListViewSlot::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CTechListViewSlot::Late_Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechListViewSlot::Render()
{

    return S_OK;
}

void CTechListViewSlot::SetViewItemInfo(const ITEM_DESC* pItemDesc, _Int iTechIndex)
{
    if (nullptr == pItemDesc)
    {
        m_pItemIcon->SetTexture(nullptr);
        m_iItemIndex = -1;
        m_iTechIndex = -1;
        m_pTechSlotFont->SetText(TEXT(""));
    }
    else
    {
        m_pItemIcon->SetTexture(CItemManager::GetInstance()->GetItemTexture(CItemManager::ITEM_TEXTURE_TYPE::INVEN, pItemDesc->iItemNum));
        m_iItemIndex = pItemDesc->iItemNum;
        m_iTechIndex = iTechIndex;
        m_pTechSlotFont->SetText(pItemDesc->szItemName);
    }
}

void CTechListViewSlot::ClikcedBindFunction(function<void(_Int, _Int)> BindFunction)
{
    m_BindClickedFunction = BindFunction;
}

void CTechListViewSlot::MouseButtonDwon()
{
    m_pGameInstance->Manager_PlaySound(TEXT("ButtonClicked.wav"), CHANNELID::EFFECT, 1.f);
    if (m_BindClickedFunction)
        m_BindClickedFunction(m_iItemIndex, m_iTechIndex);
}

HRESULT CTechListViewSlot::ADD_Components()
{
    _float3 vScale = m_pTransformCom->GetScale();
    CItemSlotIcon::ITEM_SLOT_ICON_DESC ItemSlotDesc = {};
    ItemSlotDesc.pParentTransform = m_pTransformCom;

    ItemSlotDesc.vScale = { vScale.x * 0.7f, vScale.y * 0.7f, 0.f };
    ItemSlotDesc.vPosition = {0.f,  ItemSlotDesc.vScale.y * 0.5f, 0.f };
    m_pItemIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pItemIcon->SetZOrder(static_cast<CUserInterface*>(m_pParent)->GetZOrder() + 1);
    if (FAILED(m_pItemIcon->Initialize(&ItemSlotDesc)))
        return E_FAIL;

    CTechListSlotFont::GAMEOBJECT_DESC ItemSlotFontDesc = {};
    ItemSlotFontDesc.pParent = this;
    ItemSlotFontDesc.vScale = { vScale.x, vScale.y * 0.2f, 0.f };
    ItemSlotFontDesc.vPosition = { 0.f,  ItemSlotDesc.vScale.y * 0.3f, 0.f };
    m_pTechSlotFont = CTechListSlotFont::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pTechSlotFont->SetZOrder(static_cast<CUserInterface*>(m_pParent)->GetZOrder() + 2);
    if (FAILED(m_pTechSlotFont->Initialize(&ItemSlotFontDesc)))
        return E_FAIL;

    ADD_Child(m_pItemIcon);
    ADD_Child(m_pTechSlotFont);

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
    Safe_Release(m_pTechSlotFont);
}
