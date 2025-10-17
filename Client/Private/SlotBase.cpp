#include "SlotBase.h"

#include "GameInstance.h"
#include "SlotImage.h"

CSlotBase::CSlotBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, SLOT_TYPE eType) :
    CBackGround(pDevice, pContext),
    m_eSlotType(eType)
{
}

CSlotBase::CSlotBase(const CSlotBase& rhs) :
    CBackGround(rhs),
    m_eSlotType(rhs.m_eSlotType)
{
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

    if (FAILED(CreateSlotImage()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
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

void CSlotBase::SwapSlot(CSlotBase* To)
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
}

HRESULT CSlotBase::CreateSlotImage()
{
    m_pSlotImage = CSlotImage::Create(m_pGraphic_Device, m_pDeviceContext);
    if (nullptr == m_pSlotImage)
        return E_FAIL;

    return S_OK;
}

CGameObject* CSlotBase::Clone(void* pArg)
{
    return nullptr;
}

void CSlotBase::Free()
{
    __super::Free();

    Safe_Release(m_pSlotImage);
}
