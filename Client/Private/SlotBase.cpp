#include "SlotBase.h"

#include "GameInstance.h"
#include "ItemSlotIcon.h"

#include "PlayerManager.h"
#include "ItemManager.h"

CSlotBase::CSlotBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, SLOT_TYPE eType) :
    CBackGround(pDevice, pContext),
    m_eSlotType(eType),
    m_pPlayerManager(CPlayerManager::GetInstance()),
    m_pItemManager(CItemManager::GetInstance())
{
    Safe_AddRef(m_pPlayerManager);
    Safe_AddRef(m_pItemManager);
}

CSlotBase::CSlotBase(const CSlotBase& rhs) :
    CBackGround(rhs),
    m_eSlotType(rhs.m_eSlotType),
    m_pPlayerManager(CPlayerManager::GetInstance()),
    m_pItemManager(CItemManager::GetInstance())
{
    Safe_AddRef(m_pPlayerManager);
    Safe_AddRef(m_pItemManager);
}

HRESULT CSlotBase::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSlotBase::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    m_bIsMouseEvent = true;
    return S_OK;
}

void CSlotBase::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CSlotBase::Late_Update(_float fDeletaTime)
{

}

HRESULT CSlotBase::Render()
{
    return S_OK;
}

void CSlotBase::SwapSlot(CSlotBase* From)
{
}

void CSlotBase::UseSlot(void* pArg)
{
}

void CSlotBase::MouseHoverEnter()
{
}

void CSlotBase::MouseHovering()
{
}

void CSlotBase::MouseHoverExit()
{
}

void CSlotBase::MouseButtonDwon()
{
}

void CSlotBase::MouseButtonPressed()
{
}

void CSlotBase::MouseButtonUp()
{
    if (m_bIsHover)
    {
        CUserInterface* pFoucusWidget = nullptr;
        CSlotBase* pToSlot = nullptr;

        m_pGameInstance->GetMouseFocus((CUserInterface**)&pFoucusWidget);
        m_pGameInstance->SetDrag(false);
        auto pItemSlot = dynamic_cast<CSlotBase*>(pFoucusWidget);
        if (pItemSlot)
            SwapSlot(pItemSlot);
    }
}

CGameObject* CSlotBase::Clone(void* pArg)
{
    return nullptr;
}

void CSlotBase::Free()
{
    __super::Free();

    Safe_Release(m_pSlotIcon);
    Safe_Release(m_pPlayerManager);
    Safe_Release(m_pItemManager);
}
