#include "PellWorkState.h"

#include "GameInstance.h"
#include "PellBoxManager.h"

CPellWorkState::CPellWorkState(const char* szStateName) :
	CState(szStateName),
	m_pGameInstance(CGameInstance::GetInstance()),
	m_pPellBoxManager(CPellBoxManager::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pPellBoxManager);
}

void CPellWorkState::OnStateEnter(void* pArg)
{
}

void CPellWorkState::OnStateExcution(_float fDeletaTime, void* pArg)
{
	if (!m_bIsGoToWork)
		PellWork();
	else
		GoToWork();

	UpdateWorkName();
}

void CPellWorkState::OnStateExit(void* pArg)
{
	Safe_Release(m_pNavigation);
	m_pActPell = nullptr;
}

void CPellWorkState::UpdateWorkName()
{
	if (m_bIsGoToWork)
	{
		m_szStateName = "walk";
	}
	else
	{
		switch (m_eWorkType)
		{
		case PELL_WORK_TYPE::COMMONE_WORK:
			m_szStateName = "CommonWork";
			break;
		case PELL_WORK_TYPE::TRANSPORT:
			m_szStateName = "Transport";
			break;
		case PELL_WORK_TYPE::MINING:
			m_szStateName = "Mining";
			break;
		}
	}
}

void CPellWorkState::GoToWork()
{
	// 여기서 내가 할수있는 작업을 찾아서 이동한다.
	// 근처에 왔으면 Work상태 돌입


}

void CPellWorkState::PellWork()
{
	// 여기서 내가 할수있는 작업을 한다.
	// 작업이 끝났다면 GoToWork 진입


}

CPellWorkState* CPellWorkState::Create(const char* szStateName)
{
	return new CPellWorkState(szStateName);
}

void CPellWorkState::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pPellBoxManager);
	Safe_Release(m_pNavigation);
}
