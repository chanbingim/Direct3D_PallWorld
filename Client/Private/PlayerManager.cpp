#include "PlayerManager.h"
#include "GameInstance.h"
#include "Level.h"

#include "ItemManager.h"
#include "ItemBase.h"

IMPLEMENT_SINGLETON(CPlayerManager);

void CPlayerManager::Initialize(void* pArg)
{
	PLAYER_MANAGER_DESC* Desc = static_cast<PLAYER_MANAGER_DESC*>(pArg);
	m_iNumEquipSlot = Desc->iNumEquipMaxSlot;
	m_iNumInvenSlots = Desc->iNumInvenMaxSlot;
	m_iMaxInvenWeight = Desc->iMaxInvenWeight;

	m_EquipSlots.resize(m_iNumEquipSlot, nullptr);
	m_pBackSlotItem.resize(m_iNumEquipSlot, nullptr);

	BindEquipSlot(1, 0);
	BindEquipSlot(2, 1);
}

void CPlayerManager::SelectEquipmentSlot(_uInt SlotIndex)
{
}

void CPlayerManager::BindEquipSlot(_uInt iSlotIndex, _uInt iItemIndex)
{
	/* None Swap Slot */
	if (nullptr == m_EquipSlots[iSlotIndex])
	{
		auto ItemData = CItemManager::GetInstance()->GetItemInfo(iItemIndex);

		if (ITEM_TYPE::EQUIPMENT == ItemData->ItemType)
		{
			auto pGameInstancce = CGameInstance::GetInstance();
			auto iLevelID = pGameInstancce->GetCurrentLevel()->GetLevelID();

			CModel* pModel = static_cast<CModel*>(pGameInstancce->Clone_Prototype(OBJECT_ID::COMPONENT, iLevelID, ItemData->szItemModelPath, nullptr));

			if (m_pBackSlotItem[iSlotIndex])
				Safe_Release(m_pBackSlotItem[iSlotIndex]);

			m_EquipSlots[iSlotIndex] = CItemBase::Create(*ItemData);
			m_pBackSlotItem[iSlotIndex] = pModel;
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

_bool CPlayerManager::GetIsAnimSelect()
{
	if (nullptr == m_EquipSlots[m_iSelectSlotIndex])
		return false;
	return m_EquipSlots[m_iSelectSlotIndex]->GetItemData().IsAnimModel;
}

_bool CPlayerManager::AddInventoryItem(_uInt iItemID, _uInt iCount)
{
	// 아이템을 검색해서 정보를 가져온다음 그 아이템의 무게랑 현재 무게 확인해서
	// 먹을지 말지 반환




	return true;
}

void CPlayerManager::RemoveInventoryItem(_uInt iSlotIndex, _uInt iCount)
{
	
}

const CItemBase*  CPlayerManager::GetSelectData()
{
	return m_EquipSlots[m_iSelectSlotIndex];
}

void CPlayerManager::Free()
{
	__super::Free();

	for (auto iter : m_EquipSlots)
		Safe_Release(iter);

	for (auto iter : m_pBackSlotItem)
		Safe_Release(iter);

	m_EquipSlots.clear();
	m_pBackSlotItem.clear();

	//for (_uInt i = 0; i < 2; ++i)
	//	Safe_Release(m_pBackSlotItem[i]);

}
