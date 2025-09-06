#pragma once
#include "Client_Define.h"
#include "Base.h"

NS_BEGIN(Engine)
class CItem;
NS_END

NS_BEGIN(Client)
class CPlayerManager : public CBase
{
	DECLARE_SINGLETON(CPlayerManager);
public :
	typedef		struct		PlayerManagerDesc
	{
		_uInt				iNumEquipMaxSlot;
		_uInt				iNumInvenMaxSlot;
		_uInt				iMaxInvenWeight;
	}PLAYER_MANAGER_DESC;

public :
	void					Initialize(void* pArg);

	/* 장비창  관련 함수 */
	// 장비 슬롯 번호를 통해서 무기 선택
	void					SelectEquipmentSlot(_uInt SlotIndex);
	
	// 장비 슬롯 양수 1 증가 음수 1감소
	// 현재 들어야하는 아이템의 모델 버퍼를 반환
	CItem*					SwapEquipmentSlot(_Int MoveFlag);
	CItem*					GetBackSlotItem(_uInt iBackSlotNum);

	// 아이템 보관 슬롯 최대개수
	_uInt					GetNumEuipSlot()	{ return m_iNumEquipSlot; }


	/* 인벤토리  관련 함수 */
	// 인벤토리 아이템 최대 보관개수
	_uInt					GetNumInvenSlot()	{ return m_iNumInvenSlots; }

	// 인벤토리에 보관된 아이템의 무게 반환
	_uInt					GetInvenWieght()	{ return m_iInvenWeight; }
	_bool					AddInventoryItem(_uInt iItemID, _uInt iCount);
	void					RemoveInventoryItem(_uInt iSlotIndex, _uInt iCount);

private :
	// 아이템을 보관할 슬롯 최대개수
	_uInt								m_iNumEquipSlot = {};

	// 아이템의 인덱스만 보관해 두기
	// 장비 슬롯이랑 연동해서 사용하면 될거같음
	vector<_uInt>						m_EquipSlots = {};

	//등뒤에 보관되어 보여줄 녀석의 VIBuffer
	CItem*								m_pBackSlotItem[2] = {};
	_uInt								m_iSelectSlotIndex = {};

	// 최대 인벤토리의 슬롯 개수
	_uInt								m_iNumInvenSlots = {};

	// 최대 인벤토리의 무개를 측정할 녀석
	LONG								m_iMaxInvenWeight = {};
	LONG								m_iInvenWeight = {};

	// 인벤토리의 정보를 보관할 녀석
	vector<pair<_uInt, _uInt>>			m_InvenSlots = {};

public :
	virtual		void					Free()	override;



};
NS_END