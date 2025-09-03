#include "FiniteStateMachine.h"
#include "State.h"

CFiniteStateMachine::CFiniteStateMachine()
{
}

CFiniteStateMachine::CFiniteStateMachine(const CFiniteStateMachine& rhs)
{
}

HRESULT CFiniteStateMachine::AddState(_uInt iIndex, const _wstring& StateTag, CState* pState)
{
	if (0 > m_MaxHeirary || m_MaxHeirary <= iIndex)
		return E_FAIL;

	CState* pFind = FindState(iIndex, StateTag);
	if (nullptr == pFind)
		m_States[iIndex].emplace(StateTag, pState);
	
	return S_OK;
}

HRESULT CFiniteStateMachine::Initialize()
{
	return S_OK;
}

void CFiniteStateMachine::Update(_float DeltaTime)
{
}

CState* CFiniteStateMachine::FindState(_uInt iIndex, const _wstring& StateTag)
{
	if (0 > m_MaxHeirary || m_MaxHeirary <= iIndex)
		return nullptr;

	auto pair = m_States[iIndex].find(StateTag);
	if (pair == m_States[iIndex].end())
		return nullptr;
	return pair->second;
}

void CFiniteStateMachine::Free()
{
	for (_uInt i = 0; i < m_MaxHeirary; ++i)
	{
		for (auto& pair : m_States[i])
			Safe_Release(pair.second);
		m_States[i].clear();
	}
}
