#pragma once

#include "Base.h"
#include "Client_Define.h"
#include "ItemStruct.h"

NS_BEGIN(Client)
class CItemManager : public CBase
{
	DECLARE_SINGLETON(CItemManager);

private :
	CItemManager() = default;
	virtual ~CItemManager() = default;

public :
	HRESULT							Initialize(const char* FilePath = "");

	// 아이템데이터를 넘겨주는등의 함수 기능을 제공
	const ITEM_DESC*				GetItemInfo(_uInt ItemID);

	// 파일의 데이터를 로드하는 기능
	HRESULT							LoadItemData(const char* FilePath);
	
private :
	// 맵을 통해서 아이템 인덱스와 아이템 구조체를 가지고있자.
	map<_uInt, ITEM_DESC>			m_Items;

public :
	virtual		void		Free() override;

};
NS_END