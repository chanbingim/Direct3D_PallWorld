#include "EquipSlot.h"

#include "GameInstance.h"

#include "ItemManager.h"
#include "ItemSlot.h"
#include "ItemBase.h"
#include "ItemSlotIcon.h"

#include "PlayerManager.h"


CEquipSlot::CEquipSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSlotBase(pDevice, pContext, SLOT_TYPE::EQUIP)

{
}

CEquipSlot::CEquipSlot(const CEquipSlot& rhs) :
    CSlotBase(rhs)
{
}

HRESULT CEquipSlot::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEquipSlot::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    EQUIP_SLOT_DESC* pEquipDesc = static_cast<EQUIP_SLOT_DESC*>(pArg);
    m_eEquipSlotType = pEquipDesc->eSlotType;
    m_iSlotNumber = pEquipDesc->iNumberSlot;

    return S_OK;
}

void CEquipSlot::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    const CItemBase* pItemBase = m_pPlayerManager->GetEquipSlotItem(m_eEquipSlotType, m_iSlotNumber);
    if (nullptr != pItemBase)
    {
        m_pItemDesc = m_pItemManager->GetItemInfo(pItemBase->GetItemData().iItemNum);
        m_pSlotIcon->SetTexture(m_pItemManager->GetItemTexture(CItemManager::ITEM_TEXTURE_TYPE::INVEN, m_pItemDesc->iItemNum));
    }
    else
    {
        m_pSlotIcon->SetTexture(nullptr);
    }
}

void CEquipSlot::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pSlotIcon->Late_Update(fDeletaTime);
}

HRESULT CEquipSlot::Render()
{
    if (FAILED(Apply_ConstantShaderResources()))
        return E_FAIL;

    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

void CEquipSlot::SwapSlot(CSlotBase* From)
{
    if (nullptr == From)
        return;
     
    if (SLOT_TYPE::EQUIP == From->GetSlotType())
    {
        auto pSlot = static_cast<CEquipSlot*>(From);
        if(pSlot->GetSlotType() == m_eEquipSlotType)
            m_pPlayerManager->SwapInventroyItem(pSlot->GetSlotType(), pSlot->GetSlotNumber(), m_eEquipSlotType, m_iSlotNumber);
    }
    else
    {
        auto pSlot = static_cast<CItemSlot*>(From);
        auto ItemDsec = pSlot->GetSlotItemInfo();
        if (ITEM_TYPE::EQUIPMENT == ItemDsec->ItemType)
        {
            if (ItemDsec->TypeDesc.EuqipDesc.Equip_Type == m_eEquipSlotType)
                m_pPlayerManager->SwapInventroyItem(EUQIP_TYPE::END, pSlot->GetSlotNumber(), m_eEquipSlotType, m_iSlotNumber);
        }
    }
    
}

void CEquipSlot::UseSlot(void* pArg)
{
}

void CEquipSlot::MouseHoverEnter()
{
}

void CEquipSlot::MouseHovering()
{
 
}

void CEquipSlot::MouseHoverExit()
{
}

void CEquipSlot::MouseButtonDwon()
{
    if (m_bIsHover)
    {
        m_pGameInstance->SetMouseFocus(this);
        m_pGameInstance->SetDrag(true);
    }

}

void CEquipSlot::MouseButtonPressed()
{
}

void CEquipSlot::MouseButtonUp()
{
    __super::MouseButtonUp();
}

HRESULT CEquipSlot::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_Category_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    CItemSlotIcon::ITEM_SLOT_ICON_DESC pItemIconDesc = {};
    pItemIconDesc.pParent = this;
    pItemIconDesc.pParentTransform = m_pTransformCom;
    pItemIconDesc.vOffset = {0, 0, 0};
    pItemIconDesc.vScale = m_pTransformCom->GetScale();
    pItemIconDesc.vScale.x -= 10.f;
    pItemIconDesc.vScale.y -= 10.f;
    pItemIconDesc.vScale.z -= 10.f;

    m_pSlotIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pSlotIcon->Initialize(&pItemIconDesc);
    m_pSlotIcon->SetZOrder(6);

    return S_OK;
}

CEquipSlot* CEquipSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEquipSlot* pEquipSlot = new CEquipSlot(pDevice, pContext);
    if (FAILED(pEquipSlot->Initalize_Prototype()))
    {
        Safe_Release(pEquipSlot);
        MSG_BOX("CREATE FAIL : EQUIP SLOT");
    }
    return pEquipSlot;
}

CUserInterface* CEquipSlot::Clone(void* pArg)
{
    CEquipSlot* pEquipSlot = new CEquipSlot(*this);
    if (FAILED(pEquipSlot->Initialize(pArg)))
    {
        Safe_Release(pEquipSlot);
        MSG_BOX("CLONE FAIL : EQUIP SLOT");
    }
    return pEquipSlot;
}

void CEquipSlot::Free()
{
    __super::Free();
}
