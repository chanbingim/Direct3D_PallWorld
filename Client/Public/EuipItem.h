#pragma once
#include "ItemBase.h"

NS_BEGIN(Client)
class CEuipItem : CItemBase
{
private :
	CEuipItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEuipItem(const CEuipItem& rhs);
	virtual ~CEuipItem() = default;

public :
	//초기화
	virtual		HRESULT						Initalize_Prototype(void* InitDesc);
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

private :
	// 여기서 장비 아이템만  사용하는 데이터를 가짐

public :
	static		CEuipItem*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt ItemID);
	virtual		void						Free() override;

};
NS_END