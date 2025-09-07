#pragma once

#include "Client_Define.h"
#include "ItemStruct.h"
#include "Base.h"

NS_BEGIN(Client)
class CItemBase : public CBase
{
private :
	CItemBase(_uInt ItemID);
	CItemBase(ITEM_DESC& ItemData);
	virtual ~CItemBase() = default;

public :
	const ITEM_DESC&						GetItemData()  const { return m_ItemDesc; }

private:
	ITEM_DESC								m_ItemDesc;

public:
	static			CItemBase*				Create(_uInt ItemID);
	static			CItemBase*				Create(ITEM_DESC ItemData);
	virtual			void					Free() override;
};
NS_END