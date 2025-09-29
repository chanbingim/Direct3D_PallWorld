#pragma once
#include "BackGround.h"
#include "TechStruct.h"

NS_BEGIN(Client)
class CTechSelectPreView;
class CTechListViewSlot;
class CTechManager;
class CItemManager;

class CTechSelectView : public CBackGround
{
private:
	CTechSelectView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechSelectView(const CTechSelectView& rhs);
	virtual ~CTechSelectView() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetViewTechType(TECH_TYPE eType);

private:
	TECH_TYPE								m_eTechType = {};
	
	CItemManager*							m_pItemManager = nullptr;
	CTechManager*							m_pTechManager = nullptr;

	CTechSelectPreView*						m_pSelectPreView = nullptr;
	vector<CTechListViewSlot*>				m_pViewSlotList = {};

	_uInt									m_iViewStartIndex = 0;
	_float4									m_vImageColor;
	const		TECH_ITEM_DESC*				m_SelectViewTechItem = nullptr;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();
	HRESULT									ADD_TechViewListSlot();

public:
	static		CTechSelectView*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END