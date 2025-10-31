#include "PellWorkState.h"

#include "GameInstance.h"
#include "PellBoxManager.h"

#include "WorkComponent.h"
#include "WorkAbleObject.h"
#include "PellBase.h"

CPellWorkState::CPellWorkState(const char* szStateName) :
	CState(szStateName),
	m_pGameInstance(CGameInstance::GetInstance()),
	m_pPellBoxManager(CPellBoxManager::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pPellBoxManager);
}


HRESULT CPellWorkState::Initialize(CPellBase* pOwner)
{
	CWorkComponent::WORK_COMPONENT_DESC WorkComDesc = {};
	WorkComDesc.pOwner = pOwner;
	m_pWorkComponent = static_cast<CWorkComponent *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::COMPONENT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_Component_Work"), &WorkComDesc));

	return S_OK;
}

void CPellWorkState::OnStateEnter(void* pArg)
{
	PELL_WORK_STATE_DESC* pWorkStateDesc = static_cast<PELL_WORK_STATE_DESC*>(pArg);

	m_pActPell = pWorkStateDesc->pActPell;
	m_pNavigation = pWorkStateDesc->pNavigation;
	m_pJobTarget = pWorkStateDesc->pTargetObject;
	m_pWorkComponent->SetWorkTargetObject(static_cast<CWorkAbleObject *>(pWorkStateDesc->pTargetObject), pWorkStateDesc->pNavigation);
	m_eWorkType = static_cast<CWorkAbleObject*>(m_pJobTarget)->GetWorkAbleType();
	m_WorkEndFunc = pWorkStateDesc->WorkEndFunc;

	m_iPhaseIndex = 0;
}

void CPellWorkState::OnStateExcution(_float fDeletaTime, void* pArg)
{
	if (1 == m_iPhaseIndex)
	{
		if (false == m_bNext)
		{
			m_pWorkComponent->NextWorkPhase();
			m_bNext = true;
		}
	}

	m_pWorkComponent->Update(fDeletaTime);
	UpdateWorkName();

	if (m_pWorkComponent->IsWorkFinished())
		m_WorkEndFunc();
}

void CPellWorkState::OnStateExit(void* pArg)
{
	if (false == m_pWorkComponent->IsWorkFinished())
		m_pPellBoxManager->Add_JobListObject(m_pActPell->GetPellInfo().eWorkType, m_pJobTarget);

	m_pActPell = nullptr;
	m_pNavigation = nullptr;
}

void CPellWorkState::UpdateWorkName()
{
	if (m_pWorkComponent->IsGotoWork())
	{
		m_szStateName = "Run";
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

CPellWorkState* CPellWorkState::Create(const char* szStateName, CPellBase* pOwner)
{
	auto pWorkState = new CPellWorkState(szStateName);
	if (FAILED(pWorkState->Initialize(pOwner)))
	{
		Safe_Release(pWorkState);
		MSG_BOX("CREATE FAIL : PELL WORK STATE");
	}
	return pWorkState;
}

void CPellWorkState::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pPellBoxManager);
	Safe_Release(m_pWorkComponent);
}
