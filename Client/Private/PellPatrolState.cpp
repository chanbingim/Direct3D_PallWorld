#include "PellPatrolState.h"
#include "ContainerObject.h"

CPellPatrolState::CPellPatrolState(const char* szStateName) :
	CState(szStateName)
{
}

void CPellPatrolState::OnStateEnter(void* pArg)
{



}

void CPellPatrolState::OnStateExcution(_float fDeletaTime, void* pArg)
{
	m_szStateName = "Walk";
}

void CPellPatrolState::OnStateExit(void* pArg)
{
}

CPellPatrolState* CPellPatrolState::Create(const char* szStateName)
{
	return new CPellPatrolState(szStateName);
}

void CPellPatrolState::Free()
{
	__super::Free();
}
