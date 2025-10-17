#include "FiniteStateMachine.h"
#include "StateLayer.h"
#include "Actor.h"

CFiniteStateMachine::CFiniteStateMachine()
{
}

CFiniteStateMachine::CFiniteStateMachine(const CFiniteStateMachine& rhs)
{
}

HRESULT CFiniteStateMachine::Initialize(void* pArg)
{
	FSM_DESC* pFsmDesc = static_cast<FSM_DESC*>(pArg);
	if (pFsmDesc)
	{
		m_pOwner = pFsmDesc->pOwner;
		m_StateLayers.reserve(pFsmDesc->iLayerSize);
	}
	
	return S_OK;
}

void CFiniteStateMachine::Update(_float DeltaTime)
{
	for (auto& iter : m_StateLayers)
	{
		if (iter)
			iter->Update(DeltaTime);
	}
}

HRESULT CFiniteStateMachine::AddLayer(const _wstring& LayerTag, CStateLayer* pStateLayer)
{
	auto pLayer = FindLayer(LayerTag);
	if (nullptr == pLayer)
	{
		size_t iIndex = m_StateLayers.size();
		m_LayerHashMap.emplace(LayerTag, (_uInt)iIndex);
		m_StateLayers.push_back(pStateLayer);
	}
	else
		return E_FAIL;

	return S_OK;
}

_Int CFiniteStateMachine::GetNumLayer(const _wstring& LayerTag)
{
	auto iter = m_LayerHashMap.find(LayerTag);
	if (iter == m_LayerHashMap.end())
		return -1;

	return iter->second;
}

void CFiniteStateMachine::ResetLayer(const _wstring& LayerTag)
{
	auto iter = m_LayerHashMap.find(LayerTag);
	if (iter == m_LayerHashMap.end())
		return;

	m_StateLayers[iter->second]->ResetLayer();
}

CStateLayer* CFiniteStateMachine::FindLayer(const _wstring& LayerTag)
{
	auto iter = m_LayerHashMap.find(LayerTag);
	if (iter == m_LayerHashMap.end())
		return nullptr;

	return m_StateLayers[iter->second];
}

void CFiniteStateMachine::Free()
{
	for (auto& iter : m_StateLayers)
		Safe_Release(iter);
}
