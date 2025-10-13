#pragma once

#include "BackGround.h"
#include "ItemStruct.h"

NS_BEGIN(Client)

class CItemSlotIcon;
class CPlayerManager;
class CItemManager;

class CSlotBase : public CBackGround
{
public :
	enum class SLOT_TYPE { EQUIP, ITEM, TECH, QUICKSLOT, END};

protected:
	CSlotBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, SLOT_TYPE eType);
	CSlotBase(const CSlotBase& rhs);
	virtual ~CSlotBase() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	const	SLOT_TYPE&						GetSlotType() { return m_eSlotType; }

	void									SetSlotNumber(_uInt iSlotNum) { m_iSlotNumber = iSlotNum; }
	_uInt									GetSlotNumber() { return m_iSlotNumber; }
	const ITEM_DESC*						GetSlotItemInfo() { return m_pItemDesc; }

protected :
	virtual		void						SwapSlot(CSlotBase* From);
	virtual		void						UseSlot(void* pArg);

	virtual		void						MouseHoverEnter();
	virtual		void						MouseHovering();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();
	virtual		void						MouseButtonPressed();
	virtual		void						MouseButtonUp();

protected :
	SLOT_TYPE								m_eSlotType = {};

	// 아이템 정보에대한 변수

	_uInt									m_iSlotNumber = {};
	const ITEM_DESC*						m_pItemDesc = {};

	CItemSlotIcon*							m_pSlotIcon = nullptr;
	CPlayerManager*							m_pPlayerManager = nullptr;
	CItemManager*							m_pItemManager = nullptr;

public:
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END