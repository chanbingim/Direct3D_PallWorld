#pragma once
#include "Client_Define.h"
#include "Client_Struct.h"
#include "Base.h"

NS_BEGIN(Engine)
class CModel;
NS_END

NS_BEGIN(Client)
class CItemBase;
class CPellBase;

class CPlayerManager : public CBase
{
	DECLARE_SINGLETON(CPlayerManager);

private:
	CPlayerManager() = default;
	virtual ~CPlayerManager() = default;

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
	void					BindEquipSlot(_uInt iSlotIndex, _uInt iItemIndex);

	// 장비 슬롯 양수 1 증가 음수 1감소
	// 현재 들어야하는 아이템의 모델 버퍼를 반환
	void					SwapEquipmentSlot(_Int MoveFlag);
	CModel*					GetBackSlotItem(_uInt iBackSlotNum);
	CModel*					GetCurrentSelectItem();
	CModel*					GetCurrentSelectItemProjecTileModel();

	_uInt					GetNumEuipSlot()	{ return m_iNumEquipSlot; }
	_bool					GetIsAnimSelect();
	_bool					GetIsAttachLeft();

	/* 인벤토리  관련 함수 */
	// 인벤토리 아이템 최대 보관개수
	_uInt					GetNumInvenSlot()	{ return m_iNumInvenSlots; }

	// 인벤토리에 보관된 아이템의 무게 반환
	_uInt					GetInvenWieght()	{ return m_iInvenWeight; }
	_bool					AddInventoryItem(_uInt iItemID, _uInt iCount);
	void					RemoveInventoryItem(_uInt iSlotIndex, _uInt iCount);

	const CHARACTER_DESC&	GetPlayerData() { return m_PlayerInfo; }
	const CItemBase*		GetSlotItemData(_uInt iIndex);
	const CItemBase*		GetProjecTileSlotItemData(_uInt iIndex);

	const CItemBase*		GetSelectItemData();
	const CItemBase*		GetSelectProjecTileItemData();

	void					BindPlayerCharacter(class CPlayer* pPlayer);
	_bool					IsAimState();

	class CPlayer*			GetCurrentPlayer() { return m_pCurrentPlayer; }
	//펠을 추가하기위한 클래스
	HRESULT					ADDOwnerPellList(CPellBase* pPellBase);

private :
	class CPlayer*						m_pCurrentPlayer = nullptr;

	//나중에 여기서 플레이어으 ㅣ모델 정보도 바꿀수있게 만들거같음
	CHARACTER_DESC						m_PlayerInfo = {};

	// 아이템을 보관할 슬롯 최대개수
	_uInt								m_iNumEquipSlot = {};

	// 아이템의 인덱스만 보관해 두기
	// 장비 슬롯이랑 연동해서 사용하면 될거같음
	vector<CItemBase*>					m_EquipSlots = {};
	vector<CModel*>						m_pBackSlotItem = {};

	// 장비 아이템에서 사용할 화살 및 총알 이런거들 애니메이션있으면
	// 보여주고 아니라면 안보여주려는 용도로 STL 선언
	vector<CItemBase*>					m_EquipProjectileSlots = {};
	vector<CModel*>						m_pBackProjectileSlotItem = {};

	// 등뒤에 보관되어 보여줄 녀석의 VIBuffer
	// 이건 매시 아이템 실 객체 클래스여야할거같음
	_Int								m_iSelectSlotIndex = {};

	// 최대 인벤토리의 슬롯 개수
	_uInt								m_iNumInvenSlots = {};

	// 최대 인벤토리의 무개를 측정할 녀석
	LONG								m_iMaxInvenWeight = {};
	LONG								m_iInvenWeight = {};

	// 인벤토리의 정보를 보관할 녀석
	vector<pair<_uInt, _uInt>>			m_InvenSlots = {};

	//소유 중인 펠의 포인터 정보를 그냥 통으로 넘겨서 관리할 예정
	vector<CPellBase*>					m_pOwnerPells = {};

private :
	HRESULT								SettingDefaultPlayerData();

public :
	virtual		void					Free()	override;



};
NS_END