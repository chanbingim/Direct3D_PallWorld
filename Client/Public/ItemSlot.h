#pragma once
#include "SlotBase.h"

NS_BEGIN(Client)
class CItemSlotIcon;
class CPlayerManager;
class CItemManager;

class CItemSlot : public CSlotBase
{
public:
	enum class ITEM_TYPE { WEAPON, HEAD, BODY, SHIELD, GLIDER, ACCESSORY, END };

	typedef struct ItemSlotDesc : GAMEOBJECT_DESC
	{


	}ITEM_SLOT_DESC;

private:
	CItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemSlot(const CItemSlot& rhs);
	virtual ~CItemSlot() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;
	void									SetSlotNumber(_uInt iSlotNum) { m_iSlotNumber = iSlotNum; }

protected:
	virtual		void						SwapSlot(CSlotBase* To);
	virtual		void						UseSlot(void* pArg);

	virtual		void						MouseHoverEnter();
	virtual		void						MouseHovering();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();
	virtual		void						MouseButtonPressed();
	virtual		void						MouseButtonUp();
	
private:
	ITEM_TYPE								m_eItemSlotType = { ITEM_TYPE::END };
	_uInt									m_iSlotNumber = {};

	CItemSlotIcon*							m_pSlotIcon = nullptr;
	CPlayerManager*							m_pPlayerManager = nullptr;
	CItemManager*							m_pItemManager = nullptr;

private :
	HRESULT									ADD_Components();

public:
	static		CItemSlot*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END