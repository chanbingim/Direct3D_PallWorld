#pragma once

#include "Base.h"
#include "Client_Define.h"
#include "ItemStruct.h"

NS_BEGIN(Engine)
class CTexture;
NS_END

NS_BEGIN(Client)
class CItemManager : public CBase
{
	DECLARE_SINGLETON(CItemManager);

private :
	CItemManager() = default;
	virtual ~CItemManager() = default;

public :
	HRESULT							Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* FilePath = "");

	// 아이템데이터를 넘겨주는등의 함수 기능을 제공
	const ITEM_DESC*				GetItemInfo(_uInt ItemID);
	const CTexture*					GetItemTexture(_uInt ItemID);

	// 파일의 데이터를 로드하는 기능
	// 일단 Bool로 해서 구별
	HRESULT							LoadItemData(_uInt bFlag, const char* FilePath);
	
private :
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pContext = nullptr;

	// 맵을 통해서 아이템 인덱스와 아이템 구조체를 가지고있자.
	map<_uInt, CTexture*>			m_ItemTextures;
	map<_uInt, ITEM_DESC>			m_Items;

private :
	HRESULT							ParseEuipData(vector<_string>& Data);
	HRESULT							ParseConsumeData(vector<_string>& Data);
	HRESULT							ParseArchitecturetData(vector<_string>& Data);

public :
	virtual		void		Free() override;

};
NS_END