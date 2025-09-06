#include "PlayerManager.h"

void CPlayerManager::Initialize(void* pArg)
{
	PLAYER_MANAGER_DESC* Desc = static_cast<PLAYER_MANAGER_DESC*>(pArg);
	m_iNumEquipSlot = Desc->iNumEquipMaxSlot;
	m_iNumInvenSlots = Desc->iNumInvenMaxSlot;
	m_iMaxInvenWeight = Desc->iMaxInvenWeight;
}

void CPlayerManager::SelectEquipmentSlot(_uInt SlotIndex)
{
}

CItem* CPlayerManager::SwapEquipmentSlot(_Int MoveFlag)
{
	if (0 < MoveFlag)
	{
		m_iSelectSlotIndex++;
		if (m_iNumEquipSlot <= m_iSelectSlotIndex)
			m_iSelectSlotIndex = 0;
	}
	else if (0 > MoveFlag)
	{
		m_iSelectSlotIndex--;
		if (0 > m_iSelectSlotIndex)
			m_iSelectSlotIndex = m_iNumEquipSlot - 1;
	}

	switch (m_iSelectSlotIndex)
	{
	case 0 :
		return m_pBackSlotItem[0];
	case 1:
		return m_pBackSlotItem[1];
	}

	//여기서 아이템의 정보를 받아오고 그정보에 포함되어있는
	//아이템의 프로토타입 이름을 받아서 생성해서 넘긴다.
	return nullptr;
}

CItem* CPlayerManager::GetBackSlotItem(_uInt iBackSlotNum)
{
	if (m_iSelectSlotIndex == iBackSlotNum)
		return nullptr;

	return m_pBackSlotItem[iBackSlotNum];
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

void CPlayerManager::Free()
{
	__super::Free();

	//for (_uInt i = 0; i < 2; ++i)
	//	Safe_Release(m_pBackSlotItem[i]);

}
