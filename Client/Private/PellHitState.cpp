#include "PellHitState.h"

CPellHitState::CPellHitState(const char* szStateName) :
	CState(szStateName)
{
}

void CPellHitState::OnStateEnter(void* pArg)
{
}

void CPellHitState::OnStateExcution(void* pArg)
{
}

void CPellHitState::OnStateExit(void* pArg)
{

}

CPellHitState* CPellHitState::Create(const char* szStateName)
{
	return new CPellHitState(szStateName);
}

void CPellHitState::Free()
{
	__super::Free();
}
