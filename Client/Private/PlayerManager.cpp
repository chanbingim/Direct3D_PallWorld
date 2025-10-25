#include "PlayerManager.h"
#include "GameInstance.h"
#include "Level.h"

#include "Player.h"
#include "PlayerStateMachine.h"
#include "PellBase.h"

#include "ItemManager.h"
#include "ItemBase.h"

#include "TechManager.h"

#include "Level.h"
#include "GamePlayHUD.h"


IMPLEMENT_SINGLETON(CPlayerManager);

void CPlayerManager::Initialize(void* pArg)
{
	PLAYER_MANAGER_DESC* Desc = static_cast<PLAYER_MANAGER_DESC*>(pArg);
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);

	m_iNumInvenSlots = Desc->iNumInvenMaxSlot;
	m_iMaxInvenWeight = Desc->iMaxInvenWeight;

	if (FAILED(SettingDefaultPlayerData()))
		return;

	vector<CItemBase*> vSlotItems;
	vector<CModel*> vSlotModels;

	vSlotItems.resize(5, nullptr);
	vSlotModels.resize(5, nullptr);

	// 무기 슬롯
	// 아이템 슬롯은 최대 4개니까 4개까지 넣자
	m_SlotSelectIndex.emplace(EUQIP_TYPE::WEAPON, make_pair(0, 4));
	m_pEquipItems.emplace(EUQIP_TYPE::WEAPON, vSlotItems);
	m_pEquipSlots.emplace(EUQIP_TYPE::WEAPON, vSlotModels);

	// 화살 이나 특정 발사체
	// 이거는 인벤토리 안에 있어야 확인이 가능할거같음
	m_SlotSelectIndex.emplace(EUQIP_TYPE::PROJECTILE, make_pair(0, 4));
	m_pEquipItems.emplace(EUQIP_TYPE::PROJECTILE, vSlotItems);
	m_pEquipSlots.emplace(EUQIP_TYPE::PROJECTILE, vSlotModels);

	// 푸드 슬롯
	m_SlotSelectIndex.emplace(EUQIP_TYPE::FOOD, make_pair(0, 5));
	m_pEquipItems.emplace(EUQIP_TYPE::FOOD, vSlotItems);
	m_pEquipSlots.emplace(EUQIP_TYPE::FOOD, vSlotModels);

	// 머리
	// 이거는 인벤토리 안에 있어야 확인이 가능할거같음
	vSlotItems.resize(1, nullptr);
	vSlotModels.resize(1, nullptr);
	m_SlotSelectIndex.emplace(EUQIP_TYPE::HEAD, make_pair(0, 1));
	m_pEquipItems.emplace(EUQIP_TYPE::HEAD, vSlotItems);
	m_pEquipSlots.emplace(EUQIP_TYPE::HEAD, vSlotModels);

	// 머리
	// 이거는 인벤토리 안에 있어야 확인이 가능할거같음
	m_SlotSelectIndex.emplace(EUQIP_TYPE::BODY, make_pair(0, 1));
	m_pEquipItems.emplace(EUQIP_TYPE::BODY, vSlotItems);
	m_pEquipSlots.emplace(EUQIP_TYPE::BODY, vSlotModels);

	// 방패
	// 이거는 인벤토리 안에 있어야 확인이 가능할거같음
	m_SlotSelectIndex.emplace(EUQIP_TYPE::SHIELD, make_pair(0, 1));
	m_pEquipItems.emplace(EUQIP_TYPE::SHIELD, vSlotItems);
	m_pEquipSlots.emplace(EUQIP_TYPE::SHIELD, vSlotModels);

	// 글라이더
	// 이거는 인벤토리 안에 있어야 확인이 가능할거같음
	m_SlotSelectIndex.emplace(EUQIP_TYPE::GLIDER, make_pair(0, 1));
	m_pEquipItems.emplace(EUQIP_TYPE::GLIDER, vSlotItems);
	m_pEquipSlots.emplace(EUQIP_TYPE::GLIDER, vSlotModels);

	// 장신구
	// 이거는 인벤토리 안에 있어야 확인이 가능할거같음
	vSlotItems.resize(2, nullptr);
	vSlotModels.resize(2, nullptr);
	m_SlotSelectIndex.emplace(EUQIP_TYPE::ACCESSORY, make_pair(0, 2));
	m_pEquipItems.emplace(EUQIP_TYPE::ACCESSORY, vSlotItems);
	m_pEquipSlots.emplace(EUQIP_TYPE::ACCESSORY, vSlotModels);

	m_iNumMaxOwnPell = 6;
	m_pOwnerPells.resize(m_iNumMaxOwnPell, nullptr);

	m_EmptySlot.iItemID = -1;
	m_EmptySlot.iItemCount = 0;
	m_InvenSlots.resize(m_iNumInvenSlots, { false, m_EmptySlot });

	//BindEquipSlot(EUQIP_TYPE::WEAPON, 3, 3);
	//BindEquipSlot(EUQIP_TYPE::WEAPON, 1, 1);
	//BindEquipSlot(EUQIP_TYPE::WEAPON, 2, 2);

	AddInventoryItem(1, 1);
	AddInventoryItem(2, 1);
	AddInventoryItem(3, 1);

	AddInventoryItem(100, 1);
}

#pragma region Equipment

_Int CPlayerManager::BindEquipSlot(EUQIP_TYPE SlotType, _uInt iSlotIndex, _uInt iItemIndex)
{
	auto SlotPair = m_pEquipSlots.find(SlotType);
	auto ItemInfoPair = m_pEquipItems.find(SlotType);

	if (SlotPair == m_pEquipSlots.end() || ItemInfoPair == m_pEquipItems.end())
		return -1;

	auto iLevelID = m_pGameInstance->GetCurrentLevel()->GetLevelID();
	auto ItemData = CItemManager::GetInstance()->GetItemInfo(iItemIndex);

	if (nullptr == ItemData)
		return -1;

	CModel* pModel = static_cast<CModel*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::COMPONENT, iLevelID, ItemData->szItemModelPath, nullptr));
	if (ITEM_TYPE::EQUIPMENT == ItemData->ItemType)
	{
		if (SlotType == ItemData->TypeDesc.EuqipDesc.Equip_Type)
		{
			if (ItemInfoPair->second[iSlotIndex])
			{
				Safe_Release(ItemInfoPair->second[iSlotIndex]);
				Safe_Release(SlotPair->second[iSlotIndex]);
			}
			
			ItemInfoPair->second[iSlotIndex] = CItemBase::Create(*ItemData);
			SlotPair->second[iSlotIndex] = pModel;

			if (EUQIP_TYPE::WEAPON == SlotType)
			{
				if (-1 < ItemData->TypeDesc.EuqipDesc.iProjectileItemIndex)
				{
					const ITEM_DESC* ProjectileItemData = CItemManager::GetInstance()->GetItemInfo(ItemData->TypeDesc.EuqipDesc.iProjectileItemIndex);
					if (nullptr != ProjectileItemData)
					{
						CModel* pProjecTileModel = static_cast<CModel*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::COMPONENT, iLevelID, ProjectileItemData->szItemModelPath, nullptr));
						auto ProjecTileSlotPair = m_pEquipSlots.find(EUQIP_TYPE::PROJECTILE);
						auto ProjecTileItemInfoPair = m_pEquipItems.find(EUQIP_TYPE::PROJECTILE);

						if (ProjecTileSlotPair == m_pEquipSlots.end() || ProjecTileItemInfoPair == m_pEquipItems.end())
							return -1;

						if (ProjecTileItemInfoPair->second[iSlotIndex])
						{
							Safe_Release(ProjecTileItemInfoPair->second[iSlotIndex]);
							Safe_Release(ProjecTileSlotPair->second[iSlotIndex]);
						}

						ProjecTileItemInfoPair->second[iSlotIndex] = CItemBase::Create(*ProjectileItemData);
						ProjecTileSlotPair->second[iSlotIndex] = pProjecTileModel;
					}
				}
			}
		}
	}
	else
	{
		// 소비슬롯은 일단 보류
		/*if (SlotType == ItemData->TypeDesc.ConsumDesc.)
		{
			if (ItemInfoPair->second[iSlotIndex])
			{
				Safe_Release(ItemInfoPair->second[iSlotIndex]);
				Safe_Release(SlotPair->second[iSlotIndex]);
			}

			ItemInfoPair->second[iSlotIndex] = CItemBase::Create(*ItemData);
			SlotPair->second[iSlotIndex] = pModel;
		}*/
	}

	return ItemData->iItemNum;
}

void CPlayerManager::SwapEquipmentSlot(EUQIP_TYPE SlotType, _Int MoveFlag)
{
	auto SelectSlotIndexPair = m_SlotSelectIndex.find(SlotType);

	if (SelectSlotIndexPair == m_SlotSelectIndex.end())
		return;

	if (0 < MoveFlag)
	{
		SelectSlotIndexPair->second.first++;
		if ((_Int)SelectSlotIndexPair->second.second <= SelectSlotIndexPair->second.first)
			SelectSlotIndexPair->second.first = 0;
	}
	else if (0 > MoveFlag)
	{
		SelectSlotIndexPair->second.first--;
		if (0 > SelectSlotIndexPair->second.first)
			SelectSlotIndexPair->second.first = SelectSlotIndexPair->second.second - 1;
	}
}

CModel* CPlayerManager::GetSlotItemModel(EUQIP_TYPE SlotType, _uInt iSlotNum)
{
	auto SlotPair = m_pEquipSlots.find(SlotType);
	auto SlotSelectPair = m_SlotSelectIndex.find(SlotType);

	if (SlotPair == m_pEquipSlots.end() || (_uInt)SlotPair->second.size() <= iSlotNum || SlotSelectPair == m_SlotSelectIndex.end())
		return nullptr;

	// 현재 선택된 슬롯과 같다면 안보여준다.
	if (iSlotNum == SlotSelectPair->second.first)
		return nullptr;

	return SlotPair->second[iSlotNum];
}

const CItemBase* CPlayerManager::GetSlotItemInfo(EUQIP_TYPE SlotType, _uInt iSlotNum)
{
	auto ItemInfoPair = m_pEquipItems.find(SlotType);

	if (ItemInfoPair == m_pEquipItems.end() || (_uInt)ItemInfoPair->second.size() <= iSlotNum)
		return nullptr;

	return ItemInfoPair->second[iSlotNum];
}

CModel* CPlayerManager::GetCurrentSlotItemModel(EUQIP_TYPE SlotType)
{
	auto SlotPair = m_pEquipSlots.find(SlotType);
	if (SlotPair == m_pEquipSlots.end())
		return nullptr;

	auto SlotSelectPair = m_SlotSelectIndex.find(SlotType);
	if (SlotSelectPair == m_SlotSelectIndex.end())
		return nullptr;

	return SlotPair->second[SlotSelectPair->second.first];
}

const CItemBase* CPlayerManager::GetCurrentSlotItemInfo(EUQIP_TYPE SlotType)
{
	auto ItemInfoPair = m_pEquipItems.find(SlotType);

	if (ItemInfoPair == m_pEquipItems.end())
		return nullptr;

	auto SlotSelectPair = m_SlotSelectIndex.find(SlotType);
	if (SlotSelectPair == m_SlotSelectIndex.end())
		return nullptr;

	return ItemInfoPair->second[SlotSelectPair->second.first];
}

_uInt CPlayerManager::GetNumEuipSlot(EUQIP_TYPE SlotType)
{
	auto SlotSelectPair = m_SlotSelectIndex.find(SlotType);

	if (SlotSelectPair == m_SlotSelectIndex.end())
		return 0;

	return SlotSelectPair->second.second;
}

#pragma endregion

#pragma region Inventory
_bool CPlayerManager::AddInventoryItem(_uInt iItemID, _uInt iCount)
{
	// 아이템을 검색해서 정보를 가져온다음 그 아이템의 무게랑 현재 무게 확인해서
	// 먹을지 말지 반환
	_Int iFindSlot = {};
	iFindSlot = Find_ItemSlot(iItemID);
	if (-1 == iFindSlot)
	{
		iFindSlot = FindEmpty_ItemSlot();
		if (-1 == iFindSlot)
			return false;
	}

	if (-1 < iFindSlot)
	{
		m_InvenSlots[iFindSlot].first = true;
		m_InvenSlots[iFindSlot].second.iItemID = iItemID;
		m_InvenSlots[iFindSlot].second.iItemCount = iCount;
	}
	return true;
}

_bool CPlayerManager::SubInventoryItem(const unordered_map<_uInt, _uInt>& ItemHash)
{
	// 아이템을 검색해서 정보를 가져온다음 그 아이템의 무게랑 현재 무게 확인해서
		// 먹을지 말지 반환
	list<pair<_uInt, _uInt>>		AbleSlotList = {};
	_Int iSlotIndex = {-1};

	for (auto& pSlot : m_InvenSlots)
	{
		++iSlotIndex;
		if (false == pSlot.first)
			continue;

		auto iter = ItemHash.find(pSlot.second.iItemID);
		if (iter == ItemHash.end())
			continue;
		else
		{
			if (iter->second <= pSlot.second.iItemCount)
				AbleSlotList.push_back(make_pair(iSlotIndex, pSlot.second.iItemCount));
			else
				return false;
		}
	}

	if (AbleSlotList.size() == ItemHash.size())
	{
		for (auto& Pair : AbleSlotList)
		{
			_uInt ItemCount = m_InvenSlots[Pair.first].second.iItemCount - Pair.second;
			if (ItemCount <= 0)
			{
				m_InvenSlots[Pair.first].second.iItemID = -1;
				m_InvenSlots[Pair.first].first = false;
			}
		}
	}
	else
		return false;

	return true;
}

void CPlayerManager::FindInventroyItem(unordered_map<_uInt, _uInt>& Itemlist)
{
	_Int iFindSlot = {};
	for (auto& pSlot : m_InvenSlots)
	{
		if (false == pSlot.first)
			continue;

		auto iter = Itemlist.find(pSlot.second.iItemID);
		if (iter == Itemlist.end())
			continue;
		else
			iter->second = pSlot.second.iItemCount;
	}
}

void CPlayerManager::RemoveInventoryItem(_uInt iSlotIndex, _uInt iCount)
{
	m_InvenSlots[iSlotIndex].first = false;
	m_InvenSlots[iSlotIndex].second.iItemID = -1;
	m_InvenSlots[iSlotIndex].second.iItemCount = 0;
}

HRESULT CPlayerManager::SwapInventroyItem(_uInt FromSlotNumber, _uInt ToSlotNumber)
{
	if (0 > FromSlotNumber || m_iNumInvenSlots <= FromSlotNumber ||
		0 > ToSlotNumber || m_iNumInvenSlots <= ToSlotNumber)
		return E_FAIL;

	swap(m_InvenSlots[ToSlotNumber], m_InvenSlots[FromSlotNumber]);
	return S_OK;
}

HRESULT CPlayerManager::SwapInventroyItem(EUQIP_TYPE eFromEquipType, _uInt FromSlotNumber, EUQIP_TYPE eToEquipType, _uInt ToSlotNumber)
{
	// 장비창에 장비가 있었는지 없었는지 알아서
	// 있었다면 장비 ID 반환
	// 없다면 -1 반환해서 스왑 이런식으로 가면될거같음
	if (eFromEquipType == eToEquipType)
	{
		auto SlotPair = m_pEquipSlots.find(eToEquipType);
		auto ItemInfoPair = m_pEquipItems.find(eToEquipType);

		if (SlotPair == m_pEquipSlots.end() || ItemInfoPair == m_pEquipItems.end())
			return E_FAIL;


		swap(SlotPair->second[ToSlotNumber], SlotPair->second[FromSlotNumber]);
		swap(ItemInfoPair->second[ToSlotNumber], ItemInfoPair->second[FromSlotNumber]);
	}
	else if(EUQIP_TYPE::END == eFromEquipType)
	{
		auto SlotPair = m_pEquipSlots.find(eToEquipType);
		auto ItemInfoPair = m_pEquipItems.find(eToEquipType);

		if (SlotPair == m_pEquipSlots.end() || ItemInfoPair == m_pEquipItems.end() ||
			0 > FromSlotNumber || m_iNumInvenSlots <= FromSlotNumber)
			return E_FAIL;

		if (nullptr != ItemInfoPair->second[ToSlotNumber])
		{
			auto FromSlotItemInfo = CItemManager::GetInstance()->GetItemInfo(m_InvenSlots[FromSlotNumber].second.iItemID);
			if (nullptr == FromSlotItemInfo)
				return E_FAIL;

			if (FromSlotItemInfo->ItemType == ItemInfoPair->second[ToSlotNumber]->GetItemData().ItemType)
			{
				if (FromSlotItemInfo->TypeDesc.EuqipDesc.Equip_Type == ItemInfoPair->second[FromSlotNumber]->GetItemData().TypeDesc.EuqipDesc.Equip_Type)
				{
					_uInt FomeSlotItemIndex = m_InvenSlots[FromSlotNumber].second.iItemID;
					m_InvenSlots[FromSlotNumber].second = { ItemInfoPair->second[ToSlotNumber]->GetItemData().iItemNum, 1 };
					_Int itemID = BindEquipSlot(eToEquipType, ToSlotNumber, FomeSlotItemIndex);
				}
				else
					return E_FAIL;
			}
			else
				return E_FAIL;
		}
		else
		{
			_Int itemID = BindEquipSlot(eToEquipType, ToSlotNumber, m_InvenSlots[FromSlotNumber].second.iItemID);
			m_InvenSlots[FromSlotNumber].first = false;
			m_InvenSlots[FromSlotNumber].second = m_EmptySlot;
		}

	
	}
	else if (EUQIP_TYPE::END == eToEquipType)
	{
		auto SlotPair = m_pEquipSlots.find(eFromEquipType);
		auto ItemInfoPair = m_pEquipItems.find(eFromEquipType);

		if (SlotPair == m_pEquipSlots.end() || ItemInfoPair == m_pEquipItems.end() ||
			0 > ToSlotNumber || m_iNumInvenSlots <= ToSlotNumber)
			return E_FAIL;

		if (m_InvenSlots[ToSlotNumber].first)
		{
			auto ToSlotItemInfo = CItemManager::GetInstance()->GetItemInfo(m_InvenSlots[ToSlotNumber].second.iItemID);
			if (nullptr == ToSlotItemInfo)
				return E_FAIL;

			if (ToSlotItemInfo->ItemType == ItemInfoPair->second[FromSlotNumber]->GetItemData().ItemType)
			{
				if (ToSlotItemInfo->TypeDesc.EuqipDesc.Equip_Type == ItemInfoPair->second[FromSlotNumber]->GetItemData().TypeDesc.EuqipDesc.Equip_Type)
				{
					_uInt toSlotItemIndex = m_InvenSlots[ToSlotNumber].second.iItemID;
					m_InvenSlots[ToSlotNumber].second = { ItemInfoPair->second[FromSlotNumber]->GetItemData().iItemNum, 1 };

					_Int itemID = BindEquipSlot(eFromEquipType, FromSlotNumber, toSlotItemIndex);
				}
				else
					return E_FAIL;
			}
			else
				return E_FAIL;
		}
		else
		{
			m_InvenSlots[ToSlotNumber].first = true;
			m_InvenSlots[ToSlotNumber].second = { ItemInfoPair->second[FromSlotNumber]->GetItemData().iItemNum, 1};

			Safe_Release(SlotPair->second[FromSlotNumber]);
			Safe_Release(ItemInfoPair->second[FromSlotNumber]);
		}
	}
	
	return S_OK;
}

const DEFAULT_SLOT_DESC& CPlayerManager::GetSlotItem(_uInt iSlotIndex)
{
	if (0 <= iSlotIndex && m_iNumInvenSlots > iSlotIndex)
	{
		if (m_InvenSlots[iSlotIndex].first)
			return m_InvenSlots[iSlotIndex].second;
	}
	return m_EmptySlot;
}

const CItemBase* CPlayerManager::GetEquipSlotItem(EUQIP_TYPE eEquipType, _uInt iSlotIndex)
{
	auto pSloItemInfoPair = m_pEquipItems.find(eEquipType);
	if (pSloItemInfoPair == m_pEquipItems.end() || (_uInt)pSloItemInfoPair->second.size() <= iSlotIndex)
		return nullptr;

	return pSloItemInfoPair->second[iSlotIndex];
}

_Int CPlayerManager::Find_ItemSlot(_uInt iItemID)
{
	_uInt iIndex{};
	auto pair = find_if(m_InvenSlots.begin(), m_InvenSlots.end(), [&](auto& SlotInfo)
		{
			if (SlotInfo.second.iItemID == iItemID)
			{
				iIndex;
				return true;
			}

			iIndex++;
			return false;
		});

	if (pair == m_InvenSlots.end())
		return -1;

	return iIndex;
}
_Int CPlayerManager::FindEmpty_ItemSlot()
{
	_uInt iIndex{};
	auto pair = find_if(m_InvenSlots.begin(), m_InvenSlots.end(), [&](auto& SlotInfo)
		{
			if (!SlotInfo.first)
			{
				iIndex;
				return true;
			}

			iIndex++;
			return false;
		});

	if (pair == m_InvenSlots.end())
		return -1;

	return iIndex;
}
#pragma endregion

#pragma region Player
void CPlayerManager::BindPlayerCharacter(CPlayer* pPlayer)
{
	m_pCurrentPlayer = pPlayer;
	m_pCurrentPlayer->SetPlayerData(&m_PlayerInfo);
}

void CPlayerManager::SelectArchitecture(_uInt iTechIndex)
{
	auto& TehcInfo = CTechManager::GetInstance()->GetTechData(iTechIndex);
	unordered_map<_uInt, _uInt>		IngredientItemHash;
	
	size_t iIngredientCount = TehcInfo.IngredientItemIDs.size();
	for (_uInt i = 0; i < iIngredientCount; ++i)
		IngredientItemHash.emplace(TehcInfo.IngredientItemIDs[i], TehcInfo.IngredientItemCounts[i]);

	_bool IsAble = SubInventoryItem(IngredientItemHash);

	// 만들수 있다면 세팅
	if (true)
	{
		m_pCurrentPlayer->SetArchitecture(CItemManager::GetInstance()->GetItemInfo(TehcInfo.ReturnItemID));

		auto GameplayHUD = dynamic_cast<CGamePlayHUD *>(m_pGameInstance->GetCurrentLevel()->GetHUD());
		if (GameplayHUD)
			GameplayHUD->UnActivePopUpUserInterface(1);
	}
}

void CPlayerManager::SetNearArchitecture(CArchitecture* pArchitecture)
{
	m_pCurrentPlayer->SetNearArchitecture(pArchitecture);
}

void CPlayerManager::SetNearPell(CPellBase* pPellBase, _float fDistance)
{
	m_pCurrentPlayer->SetNearPell(pPellBase, fDistance);
}

_bool CPlayerManager::IsPlayerAnimming()
{
	CPlayerStateMachine::PLAYER_STATE playerState;
	m_pCurrentPlayer->GetPlayerState(&playerState);

	return playerState.bIsAiming;
}

HRESULT CPlayerManager::SettingDefaultPlayerData()
{
	m_PlayerInfo.CurHealth = m_PlayerInfo.MaxHealth = 200.f;
	m_PlayerInfo.CurHunger = m_PlayerInfo.MaxHunger = 100.f;
	m_PlayerInfo.CurStemina = m_PlayerInfo.MaxStemina = 50.f;
	m_PlayerInfo.ShieldPoint = m_PlayerInfo.MaxShiledPoint = 100;
	m_PlayerInfo.WorkSpeed = 3.f;
	m_PlayerInfo.iLevel = 10;

	return S_OK;
}
#pragma endregion

#pragma region Pell Inven
HRESULT CPlayerManager::ADDOwnerPellList(CPellBase* pPellBase)
{
	_bool bNotOverlap = true;
	auto iter = find_if(m_pOwnerPells.begin(), m_pOwnerPells.end(), [&](CPellBase* pPell)
		{
			if (pPell == pPellBase)
				bNotOverlap = false;

			if (nullptr == pPell)
				return true;
			return false;
		});

	if (iter == m_pOwnerPells.end() || !bNotOverlap)
		return E_FAIL;

	if (ACTOR_TEAM::NEUTRAL == pPellBase->GetPellTeam())
	{
		pPellBase->ChangePellTeam(ACTOR_TEAM::FRENDLY);
		pPellBase->ChangePellStorageType(PELL_STORAGE_STATE::PLAYER_INVEN);
		*iter = pPellBase;
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerManager::ADDPellList(const PELL_INFO& PellInfo, _Int iInvenSlotID)
{
	// 생성해서 넘기자 
	_uInt iCurLevelID = m_pGameInstance->GetCurrentLevel()->GetLevelID();
	CPellBase::PELL_BASE_DESC PellDesc = {};
	PellDesc.vScale = { 1.f, 1.f, 1.f };
	PellDesc.vPosition = m_pCurrentPlayer->GetTransform()->GetPosition();

	PellDesc.bIsPellData = true;
	PellDesc.PellInfo = PellInfo;
	PellDesc.PellInfo.ePellStorageState = PELL_STORAGE_STATE::PLAYER_INVEN;
	auto pBase = m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, iCurLevelID, PellDesc.PellInfo.szPrototyeName, &PellDesc);
	if (nullptr == pBase)
		return E_FAIL;

	m_pGameInstance->Add_GameObject_ToLayer(iCurLevelID, TEXT("Layer_GamePlay_Pell"), static_cast<CGameObject *>(pBase));
	m_pOwnerPells[iInvenSlotID] = static_cast<CPellBase*>(pBase);
	return S_OK;
}

const CPellBase* CPlayerManager::GetSelectPellInfomation()
{
	return m_pOwnerPells[m_iSelectPellIndex];
}

const CPellBase* CPlayerManager::GetPellInfomation(_uInt iIndex)
{
	return m_pOwnerPells[iIndex];
}

_bool CPlayerManager::LoadPellInfomation(_uInt iIndex, PELL_INFO* pOutPellInfo)
{
	CPellBase* pPellBase = m_pOwnerPells[iIndex];
	if (pPellBase)
	{
		*pOutPellInfo = pPellBase->GetPellInfo();
		pPellBase->SetDead(true);
		m_pOwnerPells[iIndex] = nullptr;
		return true;
	}
	else
		return false;
}

void CPlayerManager::UpdateSelectPellIndex(_Int vDir)
{
	if (m_pOwnerPells[m_iSelectPellIndex])
	{
		auto PreOwnerPell = m_pOwnerPells[m_iSelectPellIndex]->GetPellInfo();
		if (PELL_STORAGE_STATE::PARTNER_PELL == PreOwnerPell.ePellStorageState)
			m_pOwnerPells[m_iSelectPellIndex]->SpawnPellFriendly();
	}

	m_iSelectPellIndex += vDir;
	if (0 > m_iSelectPellIndex)
		m_iSelectPellIndex = m_iNumMaxOwnPell - 1;
	else if (m_iNumMaxOwnPell <= m_iSelectPellIndex)
		m_iSelectPellIndex = 0;
}

void CPlayerManager::GetLeftRightSelectIndex(_Int* pLeftIndex, _Int* pSelectIndex, _Int* pRightIndex)
{
	if (0 > m_iSelectPellIndex - 1)
		*pLeftIndex = m_iNumMaxOwnPell - 1;
	else
		*pLeftIndex = m_iSelectPellIndex - 1;

	*pSelectIndex = m_iSelectPellIndex;

	if (m_iNumMaxOwnPell <= m_iSelectPellIndex + 1)
		*pRightIndex = 0;
	else
		*pRightIndex = m_iSelectPellIndex + 1;
}

void CPlayerManager::SpawnSelectPell()
{
	CPellBase* pSelectPell = m_pOwnerPells[m_iSelectPellIndex];
	if(pSelectPell)
		pSelectPell->SpawnPellFriendly();
}

HRESULT CPlayerManager::SwapInventroyPal(_uInt FromSlotNumber, _uInt ToSlotNumber)
{
	swap(m_pOwnerPells[FromSlotNumber], m_pOwnerPells[ToSlotNumber]);
	return S_OK;
}

void CPlayerManager::StorePartnerPal()
{
	CPellBase* pSelectPell = m_pOwnerPells[m_iSelectPellIndex];
	if (pSelectPell)
	{
		auto palInfo = pSelectPell->GetPellInfo();
		if (PELL_STORAGE_STATE::PARTNER_PELL == palInfo.ePellStorageState)
			pSelectPell->SpawnPellFriendly();
	}
}
#pragma endregion

void CPlayerManager::Free()
{
	__super::Free();

	for (auto Pair : m_pEquipItems)
	{
		for (auto pItemBase : Pair.second)
			Safe_Release(pItemBase);
	}

	for (auto Pair : m_pEquipSlots)
	{
		for (auto pModel : Pair.second)
			Safe_Release(pModel);
	}

	Safe_Release(m_pGameInstance);
	m_pEquipItems.clear();
	m_pEquipSlots.clear();
}