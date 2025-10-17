#include "PellPatrolState.h"
#include "ContainerObject.h"

CPellPatrolState::CPellPatrolState(const char* szStateName) :
	CState(szStateName)
{
}

void CPellPatrolState::OnStateEnter(void* pArg)
{



}

void CPellPatrolState::OnStateExcution(void* pArg)
{
	auto pOwner = static_cast<CContainerObject*>(pArg);
	//여기서 대충 속도에 의해 비교
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
