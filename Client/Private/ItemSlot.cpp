#include "ItemSlot.h"

#include "GameInstance.h"

#include "ItemManager.h"
#include "ItemSlotIcon.h"

#include "PlayerManager.h"

CItemSlot::CItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSlotBase(pDevice, pContext, SLOT_TYPE::ITEM)
{

}

CItemSlot::CItemSlot(const CItemSlot& rhs) :
    CSlotBase(rhs)
{
}

HRESULT CItemSlot::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemSlot::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    

    return S_OK;
}

void CItemSlot::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    const DEFAULT_SLOT_DESC& pSlotInfo = m_pPlayerManager->GetSlotItem(m_iSlotNumber);
    m_pItemDesc = m_pItemManager->GetItemInfo(pSlotInfo.iItemID);
    m_pSlotIcon->SetTexture(m_pItemManager->GetItemTexture(pSlotInfo.iItemID));
}

void CItemSlot::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pSlotIcon->Late_Update(fDeletaTime);
}

HRESULT CItemSlot::Render()
{
    if (FAILED(Apply_ConstantShaderResources()))
        return E_FAIL;

    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

void CItemSlot::SwapSlot(CSlotBase* From)
{
    if (nullptr == From)
        return;

    auto pSlot = static_cast<CItemSlot*>(From);
    if (ITEM_TYPE::END == m_eItemSlotType)
    {
        m_pPlayerManager->SwapInventroyItem(pSlot->GetSlotNumber(), m_iSlotNumber);
    }
}

void CItemSlot::UseSlot(void* pArg)
{

}

void CItemSlot::MouseHoverEnter()
{

}

void CItemSlot::MouseHovering()
{

   
}

void CItemSlot::MouseHoverExit()
{

}

void CItemSlot::MouseButtonDwon()
{
    m_pGameInstance->SetMouseFocus(this);
}

void CItemSlot::MouseButtonPressed()
{
}

void CItemSlot::MouseButtonUp()
{
    if (m_bIsHover)
    {
        CUserInterface* pFoucusWidget = nullptr;
        CItemSlot* pToSlot = nullptr;
    
        m_pGameInstance->GetMouseFocus((CUserInterface**)&pFoucusWidget);
        m_pGameInstance->SetDrag(false);

        auto pItemSlot = dynamic_cast<CSlotBase*>(pFoucusWidget);
        if(pItemSlot)
            SwapSlot(pItemSlot);
    }
}

HRESULT CItemSlot::ADD_Components()
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
    pItemIconDesc.vScale = m_pTransformCom->GetScale();
    pItemIconDesc.vScale.x -= 10.f;
    pItemIconDesc.vScale.y -= 10.f;
    pItemIconDesc.vScale.z -= 10.f;

    m_pSlotIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pSlotIcon->Initialize(&pItemIconDesc);
    m_pSlotIcon->SetZOrder(4);

    return S_OK;
}

CItemSlot* CItemSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItemSlot* pItemSlot = new CItemSlot(pDevice, pContext);
    if (FAILED(pItemSlot->Initalize_Prototype()))
    {
        Safe_Release(pItemSlot);
        MSG_BOX("CREATE FAIL : ITEM SLOT");
    }
    return pItemSlot;
}

CUserInterface* CItemSlot::Clone(void* pArg)
{
    CItemSlot* pItemSlot = new CItemSlot(*this);
    if (FAILED(pItemSlot->Initialize(pArg)))
    {
        Safe_Release(pItemSlot);
        MSG_BOX("CLONE FAIL : ITEM SLOT");
    }
    return pItemSlot;
}

void CItemSlot::Free()
{
    __super::Free();


}
