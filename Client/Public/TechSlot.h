#pragma once
#include "SlotBase.h"
#include "TechStruct.h"

NS_BEGIN(Client)

class CTechSlotNameBar;
class CTechManager;
class CTechSlotTitleBar;
class CItemSlotIcon;

class CTechSlot : public CSlotBase
{
private:
	CTechSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechSlot(const CTechSlot& rhs);
	virtual ~CTechSlot() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetSlotItem(const TECH_ITEM_DESC& TehcItem);

protected:
	virtual		void						UseSlot(void* pArg);

	virtual		void						MouseHoverEnter();
	virtual		void						MouseHovering();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();
	virtual		void						MouseButtonPressed();
	virtual		void						MouseButtonUp();

private:
	const	TECH_ITEM_DESC*					m_TechItem;

	CTechManager*							m_pTechManager = nullptr;
	CTechSlotTitleBar*						m_pSlotTitleBar = nullptr;
	CItemSlotIcon*							m_pSlotIcon = nullptr;
	CTechSlotNameBar*						m_pSlotItemName = nullptr;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

	const WCHAR*							GetTechTypeName(TECH_TYPE eTechType);

public:
	static		CTechSlot*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END