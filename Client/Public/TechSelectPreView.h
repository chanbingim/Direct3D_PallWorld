#pragma once
#include "BackGround.h"
#include "ItemStruct.h"

NS_BEGIN(Client)
class CItemSlotIcon;
class CItemManager;
class CTechPreViewFont;

class CTechSelectPreView : public CBackGround
{
private:
	CTechSelectPreView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechSelectPreView(const CTechSelectPreView& rhs);
	virtual ~CTechSelectPreView() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetViewItemInfo(const ITEM_DESC* ItemInfo);

private:
	const ITEM_DESC*						m_pItemInfo = nullptr;

	CItemSlotIcon*							m_pPreViewIcon = nullptr;
	CItemManager*							m_pItemManager = nullptr;
	CTechPreViewFont*						m_pPreViewFont = nullptr;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static		CTechSelectPreView*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END