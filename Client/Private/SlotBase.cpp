#include "SlotBase.h"

CSlotBase::CSlotBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CSlotBase::CSlotBase(const CSlotBase& rhs) :
    CBackGround(rhs)
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

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    return S_OK;
}

void CSlotBase::Update(_float fDeletaTime)
{

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

HRESULT CSlotBase::ADD_Components()
{
    return E_NOTIMPL;
}

CSlotBase* CSlotBase::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

CGameObject* CSlotBase::Clone(void* pArg)
{
    return nullptr;
}

void CSlotBase::Free()
{
}
