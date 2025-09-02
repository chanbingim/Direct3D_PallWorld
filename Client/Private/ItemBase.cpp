#include "ItemBase.h"

CItemBase::CItemBase(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CContainerObject(pGraphic_Device, pDeviceContext)
{
}

CItemBase::CItemBase(const CItemBase& rhs) :
    CContainerObject(rhs),
    m_ItemNum(rhs.m_ItemNum),
    m_ItemType(rhs.m_ItemType)
{
}

HRESULT CItemBase::Initalize_Prototype(void* InitDesc)
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    INIT_ITEM_DESC* Desc = static_cast<INIT_ITEM_DESC*>(InitDesc);

    m_ItemNum = Desc->iItemNum;
    m_ItemType = Desc->eItemType;

    return S_OK;
}

HRESULT CItemBase::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CItemBase::Priority_Update(_float fDeletaTime)
{

}

void CItemBase::Update(_float fDeletaTime)
{

}

void CItemBase::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);

}

HRESULT CItemBase::Render()
{
    return S_OK;
}

CGameObject* CItemBase::Clone(void* pArg)
{
    return nullptr;
}

void CItemBase::Free()
{
    __super::Free();
}
