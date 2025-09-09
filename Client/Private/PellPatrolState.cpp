#include "PellPatrolState.h"

CPellPatrolState::CPellPatrolState(const char* szStateName) :
	CState(szStateName)
{
}

void CPellPatrolState::OnStateEnter(void* pArg)
{
}

void CPellPatrolState::OnStateExcution(void* pArg)
{
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
