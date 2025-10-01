#pragma once
#include "SlotBase.h"

NS_BEGIN(Client)

class CEquipSlot : public CSlotBase
{
public :
	typedef struct EquipSlotDesc : public GAMEOBJECT_DESC
	{
		EUQIP_TYPE			eSlotType;
		_uInt				iNumberSlot;
	}EQUIP_SLOT_DESC;

private :
	CEquipSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEquipSlot(const CEquipSlot& rhs);
	virtual ~CEquipSlot() = default;

public :
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	const		EUQIP_TYPE&					GetSlotType() { return m_eEquipSlotType; }

protected:
	virtual		void						SwapSlot(CSlotBase* From);
	virtual		void						UseSlot(void* pArg);

	virtual		void						MouseHoverEnter();
	virtual		void						MouseHovering();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();
	virtual		void						MouseButtonPressed();
	virtual		void						MouseButtonUp();

private :
	EUQIP_TYPE								m_eEquipSlotType = { EUQIP_TYPE::END };

private :
	HRESULT									ADD_Components();

public :
	static		CEquipSlot*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END