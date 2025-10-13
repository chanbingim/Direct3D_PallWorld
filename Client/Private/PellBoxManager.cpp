#include "PellBoxManager.h"

#include "GameInstance.h"
#include "Level.h"

#include "PellBase.h"
#include "PalBox.h"

IMPLEMENT_SINGLETON(CPellBoxManager);

CPellBoxManager::CPellBoxManager() :
	m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

void CPellBoxManager::Initialize_Manager()
{

}

void CPellBoxManager::Add_PalBox(CPalBox* pPalBox)
{
	auto iter = find(m_PalBoxList.begin(), m_PalBoxList.end(), pPalBox);
	if (iter == m_PalBoxList.end())
	{
		Safe_AddRef(pPalBox);
		m_PalBoxList.push_back(pPalBox);
	}
}

void CPellBoxManager::Remove_PalBox(CPalBox* pPalBox)
{
	auto iter = find(m_PalBoxList.begin(), m_PalBoxList.end(), pPalBox);
	if (iter != m_PalBoxList.end())
	{
		Safe_Release(*iter);
		m_PalBoxList.erase(iter);
	}
}

void CPellBoxManager::Add_JobListObject(PELL_WORK_TYPE eWorkType, CGameObject* pObject)
{
	for (auto pPalBox : m_PalBoxList)
	{
		_float3 vWorkAbleObjectPos = pObject->GetTransform()->GetPosition();
		_float3 vPalBoxPos = pPalBox->GetTransform()->GetPosition();

		_float  fLenght = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPalBoxPos) - XMLoadFloat3(&vWorkAbleObjectPos)));
		if (fLenght <= pPalBox->GetPalBoxArea())
		{
			pPalBox->Add_JobListObject(static_cast<CWorkAbleObject*>(pObject)->GetWorkAbleType(), pObject);
			break;
		}
	}
}

void CPellBoxManager::Remove_JobListObject(PELL_WORK_TYPE eWorkType, CGameObject* pObject)
{
	for (auto pPalBox : m_PalBoxList)
		pPalBox->Remove_JobListObject(static_cast<CWorkAbleObject*>(pObject)->GetWorkAbleType(), pObject);
}

void CPellBoxManager::ShowPalBoxUserInterface(CPalBox* pPalBox)
{
	//이건 UI 표현을 위한 데이터
	m_pSelectPalBox = pPalBox;
}

void CPellBoxManager::StorePalBox(const PELL_INFO& PellInfo, _Int iStoreID)
{
	if (nullptr == m_pSelectPalBox)
		return;

	m_pSelectPalBox->StorePalBox(PellInfo, iStoreID);
}

const PELL_INFO* CPellBoxManager::GetPalBoxInfo(_uInt iStoreID)
{
	if (nullptr == m_pSelectPalBox)
		return nullptr;

	return m_pSelectPalBox->GetPalBoxInfo(iStoreID);
}

HRESULT CPellBoxManager::LoadPalBox(_uInt iStoreID, PELL_INFO* pOutPalInfo)
{
	if (nullptr == m_pSelectPalBox)
		return E_FAIL;

	return m_pSelectPalBox->LoadPalBox(iStoreID, pOutPalInfo);
}

void CPellBoxManager::Add_WorkPalList(_uInt iStoreID)
{
	if (nullptr == m_pSelectPalBox)
		return;

	m_pSelectPalBox->Add_WorkPalList(iStoreID);
}

void CPellBoxManager::Add_WorkPalList(const PELL_INFO& PellInfo, _Int WorkSlotID)
{
	if (nullptr == m_pSelectPalBox)
		return;

	m_pSelectPalBox->Add_WorkPalList(PellInfo, WorkSlotID);
}

const PELL_INFO* CPellBoxManager::GetWorkPalInfo(_uInt iWorkIndex)
{
	if (nullptr == m_pSelectPalBox)
		return nullptr;

	return m_pSelectPalBox->GetWorkPalInfo(iWorkIndex);
}

void CPellBoxManager::Remove_WorkPalList(CPellBase* pPellBase)
{
	if (nullptr == m_pSelectPalBox)
		return;

	m_pSelectPalBox->Remove_WorkPalList(pPellBase);
}

void CPellBoxManager::Load_WorkPalList(_uInt iStoreID, PELL_INFO* pOutPalInfo)
{
	if (nullptr == m_pSelectPalBox)
		return;

	m_pSelectPalBox->Load_WorkPalList(iStoreID, pOutPalInfo);
}

void CPellBoxManager::SwapPalBox(_uInt iSlotType, _uInt iToSlotIndex, _uInt iFromSlotIndex)
{
	if (nullptr == m_pSelectPalBox)
		return;

	m_pSelectPalBox->SwapPalBox(iSlotType, iToSlotIndex, iFromSlotIndex);
}

void CPellBoxManager::Free()
{
	__super::Free();

	for (auto pPalBox : m_PalBoxList)
		Safe_Release(pPalBox);
	m_PalBoxList.clear();


	Safe_Release(m_pGameInstance);
}
