#include "PlayerManager.h"
#include "GameInstance.h"
#include "Level.h"

#include "Player.h"
#include "PlayerStateMachine.h"
#include "PellBase.h"

#include "ItemManager.h"
#include "ItemBase.h"

IMPLEMENT_SINGLETON(CPlayerManager);

void CPlayerManager::Initialize(void* pArg)
{
	PLAYER_MANAGER_DESC* Desc = static_cast<PLAYER_MANAGER_DESC*>(pArg);
	m_iNumEquipSlot = Desc->iNumEquipMaxSlot;
	m_iNumInvenSlots = Desc->iNumInvenMaxSlot;
	m_iMaxInvenWeight = Desc->iMaxInvenWeight;

	if (FAILED(SettingDefaultPlayerData()))
		return;

	m_EquipSlots.resize(m_iNumEquipSlot, nullptr);
	m_pBackSlotItem.resize(m_iNumEquipSlot, nullptr);

	m_EquipProjectileSlots.resize(m_iNumEquipSlot, nullptr);
	m_pBackProjectileSlotItem.resize(m_iNumEquipSlot, nullptr);

	m_iNumMaxOwnPell = 6;
	m_pOwnerPells.resize(m_iNumMaxOwnPell, nullptr);

	BindEquipSlot(3, 3);
	BindEquipSlot(1, 1);
	BindEquipSlot(2, 2);
}

#pragma region Equipment
void CPlayerManager::SelectEquipmentSlot(_uInt SlotIndex)
{
}

void CPlayerManager::BindEquipSlot(_uInt iSlotIndex, _uInt iItemIndex)
{
	/* None Swap Slot */
	if (nullptr == m_EquipSlots[iSlotIndex])
	{
		auto ItemData = CItemManager::GetInstance()->GetItemInfo(iItemIndex);
		const ITEM_DESC* ProejectileItemData = nullptr;
		if (-1 < ItemData->TypeDesc.EuqipDesc.iProjectileItemIndex)
			ProejectileItemData = CItemManager::GetInstance()->GetItemInfo(ItemData->TypeDesc.EuqipDesc.iProjectileItemIndex);

		if (ITEM_TYPE::EQUIPMENT == ItemData->ItemType)
		{
			auto pGameInstancce = CGameInstance::GetInstance();
			auto iLevelID = pGameInstancce->GetCurrentLevel()->GetLevelID();

			CModel* pModel = static_cast<CModel*>(pGameInstancce->Clone_Prototype(OBJECT_ID::COMPONENT, iLevelID, ItemData->szItemModelPath, nullptr));
			if (m_pBackSlotItem[iSlotIndex])
			{
				Safe_Release(m_EquipSlots[iSlotIndex]);
				Safe_Release(m_pBackSlotItem[iSlotIndex]);
			}

			m_EquipSlots[iSlotIndex] = CItemBase::Create(*ItemData);
			m_pBackSlotItem[iSlotIndex] = pModel;

			if (nullptr != ProejectileItemData)
			{
				CModel* pProjecTileModel = static_cast<CModel*>(pGameInstancce->Clone_Prototype(OBJECT_ID::COMPONENT, iLevelID, ProejectileItemData->szItemModelPath, nullptr));
				if (m_pBackProjectileSlotItem[iSlotIndex])
				{
					Safe_Release(m_EquipProjectileSlots[iSlotIndex]);
					Safe_Release(m_pBackProjectileSlotItem[iSlotIndex]);
				}

				m_EquipProjectileSlots[iSlotIndex] = CItemBase::Create(*ProejectileItemData);
				m_pBackProjectileSlotItem[iSlotIndex] = pProjecTileModel;
			}
		}
	}
}

void CPlayerManager::SwapEquipmentSlot(_Int MoveFlag)
{
	if (0 >= m_iNumEquipSlot)
		return;

	if (0 < MoveFlag)
	{
		m_iSelectSlotIndex++;
		if ((_Int)m_iNumEquipSlot <= m_iSelectSlotIndex)
			m_iSelectSlotIndex = 0;
	}
	else if (0 > MoveFlag)
	{
		m_iSelectSlotIndex--;
		if (0 > m_iSelectSlotIndex)
			m_iSelectSlotIndex = m_iNumEquipSlot - 1;
	}
}

CModel* CPlayerManager::GetBackSlotItem(_uInt iBackSlotNum)
{
	if (m_iSelectSlotIndex == iBackSlotNum || m_pBackSlotItem.size() <= iBackSlotNum || 0 > iBackSlotNum)
		return nullptr;

	return m_pBackSlotItem[iBackSlotNum];
}

CModel* CPlayerManager::GetCurrentSelectItem()
{
	return m_pBackSlotItem[m_iSelectSlotIndex];
}

CModel* CPlayerManager::GetCurrentSelectItemProjecTileModel()
{
	return m_pBackProjectileSlotItem[m_iSelectSlotIndex];;
}

const CItemBase* CPlayerManager::GetSlotItemData(_uInt iIndex)
{
	return m_EquipSlots[iIndex];
}

const CItemBase* CPlayerManager::GetProjecTileSlotItemData(_uInt iIndex)
{
	return m_EquipProjectileSlots[iIndex];
}

const CItemBase* CPlayerManager::GetSelectItemData()
{
	return m_EquipSlots[m_iSelectSlotIndex];
}

const CItemBase* CPlayerManager::GetSelectProjecTileItemData()
{
	return m_EquipProjectileSlots[m_iSelectSlotIndex];
}
#pragma endregion

#pragma region Inventory
_bool CPlayerManager::AddInventoryItem(_uInt iItemID, _uInt iCount)
{
	// 아이템을 검색해서 정보를 가져온다음 그 아이템의 무게랑 현재 무게 확인해서
	// 먹을지 말지 반환




	return true;
}

void CPlayerManager::RemoveInventoryItem(_uInt iSlotIndex, _uInt iCount)
{

}
#pragma endregion

#pragma region Player
void CPlayerManager::BindPlayerCharacter(CPlayer* pPlayer)
{
	m_pCurrentPlayer = pPlayer;
	m_pCurrentPlayer->SetPlayerData(&m_PlayerInfo);
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

	if (CPellBase::PELL_TEAM::NEUTRAL == pPellBase->GetPellTeam())
	{
		pPellBase->ChangePellTeam(CPellBase::PELL_TEAM::FRENDLY);
		*iter = pPellBase;
	}
	else
		return E_FAIL;

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

void CPlayerManager::UpdateSelectPellIndex(_uInt vDir)
{
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
#pragma endregion

void CPlayerManager::Free()
{
	__super::Free();

	for (auto iter : m_EquipSlots)
		Safe_Release(iter);

	for (auto iter : m_pBackSlotItem)
		Safe_Release(iter);

	for (auto iter : m_EquipProjectileSlots)
		Safe_Release(iter);

	for (auto iter : m_pBackProjectileSlotItem)
		Safe_Release(iter);

	m_EquipSlots.clear();
	m_pBackSlotItem.clear();
	m_EquipProjectileSlots.clear();
	m_pBackProjectileSlotItem.clear();
}