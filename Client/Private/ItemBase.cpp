#include "ItemBase.h"
#include "ItemManager.h"

CItemBase::CItemBase(_uInt ItemID)
{
	auto ItemData = CItemManager::GetInstance()->GetItemInfo(ItemID);
    m_ItemDesc = *ItemData;
}

CItemBase::CItemBase(ITEM_DESC& ItemData) : m_ItemDesc(ItemData)
{

}


CItemBase* CItemBase::Create(_uInt ItemID)
{
    return new CItemBase(ItemID);
}

CItemBase* CItemBase::Create(ITEM_DESC ItemData)
{
    return new CItemBase(ItemData);
}

void CItemBase::Free()
{
    __super::Free();
}
