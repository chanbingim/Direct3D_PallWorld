#pragma once
#include "Client_Define.h"
#include "Client_Struct.h"

#include "PellStructData.h"
#include "ItemStruct.h"
#include "Base.h"

NS_BEGIN(Engine)
class CModel;
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CItemBase;
class CPellBase;
class CArchitecture;

class CPlayerManager : public CBase
{
	DECLARE_SINGLETON(CPlayerManager);

private:
	CPlayerManager() = default;
	virtual ~CPlayerManager() = default;

public :
	typedef		struct		PlayerManagerDesc
	{
		_uInt				iNumInvenMaxSlot;
		_uInt				iMaxInvenWeight;
	}PLAYER_MANAGER_DESC;

public :
	void					Initialize(void* pArg);

#pragma region EuipMent Function
	/* 장비창  관련 함수 */
	// 장비 슬롯 번호를 통해서 무기 선택
	_Int					BindEquipSlot(EUQIP_TYPE SlotType, _uInt iSlotIndex, _uInt iItemIndex);

	// 장비 슬롯 양수 1 증가 음수 1감소
	// 현재 들어야하는 아이템의 모델 버퍼를 반환
	void					SwapEquipmentSlot(EUQIP_TYPE SlotType, _Int MoveFlag);

	CModel*					GetSlotItemModel(EUQIP_TYPE SlotType, _uInt iSlotNum);
	const CItemBase*		GetSlotItemInfo(EUQIP_TYPE SlotType, _uInt iSlotNum);

	CModel*					GetCurrentSlotItemModel(EUQIP_TYPE SlotType);
	const	CItemBase*		GetCurrentSlotItemInfo(EUQIP_TYPE SlotType);

	_uInt					GetNumEuipSlot(EUQIP_TYPE SlotType);
#pragma endregion

#pragma region Iven Function
	/* 인벤토리  관련 함수 */
	// 인벤토리 아이템 최대 보관개수
	_uInt						GetNumInvenSlot() { return m_iNumInvenSlots; }

	// 인벤토리에 보관된 아이템의 무게 반환
	_uInt						GetInvenWieght() { return m_iInvenWeight; }
	_bool						AddInventoryItem(_uInt iItemID, _uInt iCount);
	_bool						SubInventoryItem(const unordered_map<_uInt, _uInt>& ItemHash);
	void						FindInventroyItem(unordered_map<_uInt, _uInt>& Itemlist);
	
	void						RemoveInventoryItem(_uInt iSlotIndex, _uInt iCount);

	HRESULT						SwapInventroyItem(_uInt FromSlotNumber, _uInt ToSlotNumber);

	//장비창에 대한 데이터
	HRESULT						SwapInventroyItem(EUQIP_TYPE eFromEquipType, _uInt FromSlotNumber, EUQIP_TYPE eToEquipType, _uInt ToSlotNumber);
	const DEFAULT_SLOT_DESC&	GetSlotItem(_uInt iSlotIndex);
	const CItemBase*			GetEquipSlotItem(EUQIP_TYPE eEquipType,_uInt iSlotIndex);
#pragma endregion

#pragma region PLATYER Function
	class CPlayer*			GetCurrentPlayer() { return m_pCurrentPlayer; }
	const CHARACTER_DESC&	GetPlayerData() { return m_PlayerInfo; }
	void					BindPlayerCharacter(class CPlayer* pPlayer);

	void					SelectArchitecture(_uInt iTechIndex);
	void					SetNearArchitecture(CArchitecture* pArchitecture);
	void					SetNearPell(CPellBase* pPellBase, _float fDistance);

	_bool					IsPlayerAnimming();
#pragma endregion
	
#pragma region PELL INVEN
	//펠을 추가하기위한 클래스
	HRESULT					ADDOwnerPellList(CPellBase* pPellBase);
	HRESULT					ADDPellList(const PELL_INFO& PellInfo, _Int iInvenSlotID = -1);

	const CPellBase*		GetSelectPellInfomation();

	const CPellBase*		GetPellInfomation(_uInt iIndex);
	_bool					LoadPellInfomation(_uInt iIndex, PELL_INFO*	pOutPellInfo);

	// 0번 가운데 -1 왼쪽 1 오른쪽
	void					UpdateSelectPellIndex(_Int vDir);
	void					GetLeftRightSelectIndex(_Int* pLeftIndex, _Int* pSelectIndex, _Int* pRightIndex);

	void					SpawnSelectPell();

	//인벤토리 안에서 스왑할때
	HRESULT					SwapInventroyPal(_uInt FromSlotNumber, _uInt ToSlotNumber);

	_uInt					GetCurrentSelectPellSlotIndex() { return m_iSelectPellIndex; }
#pragma endregion

private :
	CGameInstance*						m_pGameInstance = nullptr;

#pragma region Player variable
	class CPlayer*						m_pCurrentPlayer = nullptr;

	//나중에 여기서 플레이어으 ㅣ모델 정보도 바꿀수있게 만들거같음
	CHARACTER_DESC						m_PlayerInfo = {};
#pragma endregion

#pragma region EuipMent Variable
	// 아이템을 보관할 슬롯 최대개수
	unordered_map<EUQIP_TYPE, pair<_Int, _uInt>>		m_SlotSelectIndex = {};
	unordered_map<EUQIP_TYPE, vector<CItemBase*>>		m_pEquipItems = {};
	unordered_map<EUQIP_TYPE, vector<CModel*>>			m_pEquipSlots = {};
#pragma endregion

#pragma region Pell Iven Variable
	//소유 중인 펠의 포인터 정보를 그냥 통으로 넘겨서 관리할 예정
	vector<CPellBase*>					m_pOwnerPells = {};

	//소유할수 있는 펠의 개수
	_uInt								m_iNumMaxOwnPell = {};

	// 현재 어떤 펠이 선택되어 있는지
	_Int								m_iSelectPellIndex = {};
#pragma endregion

#pragma region Iven variable
	// 최대 인벤토리의 슬롯 개수
	_uInt								m_iNumInvenSlots = {};

	// 최대 인벤토리의 무개를 측정할 녀석
	LONG								m_iMaxInvenWeight = {};
	LONG								m_iInvenWeight = {};

	//빈정보			
	DEFAULT_SLOT_DESC					m_EmptySlot = {};
	// 인벤토리의 정보를 보관할 녀석
	vector<pair<_bool, DEFAULT_SLOT_DESC>>		m_InvenSlots = {};
#pragma endregion

private :
	HRESULT								SettingDefaultPlayerData();

	//슬롯 번호를 반환
	_Int								Find_ItemSlot(_uInt iItemID);
	_Int								FindEmpty_ItemSlot();

public :
	virtual		void					Free()	override;



};
NS_END