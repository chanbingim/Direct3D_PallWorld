#include "PellHitState.h"

CPellHitState::CPellHitState(const char* szStateName) :
	CState(szStateName)
{
}

void CPellHitState::OnStateEnter(void* pArg)
{
	m_bStateAnimLoop = false;
}

void CPellHitState::OnStateExcution(_float fDeletaTime, void* pArg)
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
