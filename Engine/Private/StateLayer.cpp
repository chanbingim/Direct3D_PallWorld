#include "StateLayer.h"
#include "State.h"

CStateLayer::CStateLayer()
{
}

CStateLayer::CStateLayer(const CStateLayer& rhs)
{
}

HRESULT CStateLayer::Initialize(void* pArg, _uInt iStateSize)
{
    m_States.reserve(iStateSize);
    return S_OK;
}

void CStateLayer::Update(_float DeltaTime, void* pArg)
{
    if (m_pCurState)
        m_pCurState->OnStateExcution(DeltaTime, pArg);
}

HRESULT CStateLayer::AddState(const _wstring& StateTag, CState* pNewState)
{
    pair<CState*, _Int> pState = FindState(StateTag);
    if (nullptr == pState.first)
    {
        size_t HashIndex = m_States.size();
        m_StatesHasMap.emplace(StateTag, (_uInt)HashIndex);
        m_States.push_back(pNewState);
    }
    else
        return E_FAIL;

    return S_OK;
}

_Int CStateLayer::GetStateNum(const _wstring& StateTag)
{
    auto iter = m_StatesHasMap.find(StateTag);
    if (iter == m_StatesHasMap.end())
        return -1;

    return (*iter).second;
}

_Int CStateLayer::GetCurrentStateNum()
{
    return m_iCurStateIndex;
}

const char* CStateLayer::GetCurStateName()
{
    if (nullptr == m_pCurState)
        return nullptr;

    return m_pCurState->GetStateName();
}

HRESULT CStateLayer::ChangeState(const _wstring& StateTag, void* pArg)
{
    pair<CState*, _Int> pNextState = FindState(StateTag);
    if (m_pCurState == pNextState.first)
        return E_FAIL;
    else
    {
        m_iCurStateIndex = pNextState.second;
        if(m_pCurState)
            m_pCurState->OnStateExit();
        pNextState.first->OnStateEnter(pArg);

        m_pCurState = pNextState.first;
    }
    return S_OK;
 }

_uInt CStateLayer::StateNextPhase()
{
    if (m_pCurState)
        return m_pCurState->NexPhase();
    return -1;
}

_uInt CStateLayer::GetCurStatePhase()
{
    if (m_pCurState)
        return m_pCurState->GetPhaseIndex();
    return -1;
}

void CStateLayer::ResetLayer()
{
    if (m_pCurState)
    {
        m_pCurState->OnStateExit();
        m_pCurState = nullptr;
    }
}

const _bool CStateLayer::GetCurrentStateAnimLoop()
{
    return m_pCurState->GetStateAnimLoop();
}

const _bool CStateLayer::GetCurrentStateLastPhase()
{
    return m_pCurState->GetLastPahse();
}

pair<CState*, _Int> CStateLayer::FindState(const _wstring& StateTag)
{
    auto iter = m_StatesHasMap.find(StateTag);
    if (iter == m_StatesHasMap.end())
        return { nullptr, -1 };

    return { m_States[(*iter).second], (*iter).second };
}

void CStateLayer::Free()
{
    __super::Free();

    for (auto& iter : m_States)
        Safe_Release(iter);

}
