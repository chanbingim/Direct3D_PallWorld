#include "PellWorkState.h"

CPellWorkState::CPellWorkState(const char* szStateName) :
	CState(szStateName)
{
}

void CPellWorkState::OnStateEnter(void* pArg)
{
}

void CPellWorkState::OnStateExcution(void* pArg)
{
}

void CPellWorkState::OnStateExit(void* pArg)
{
}

CPellWorkState* CPellWorkState::Create(const char* szStateName)
{
	return new CPellWorkState(szStateName);
}

void CPellWorkState::Free()
{
	__super::Free();
}
