#include "Prototype_Manager.h"

HRESULT CPrototype_Manager::Initialize(_uInt iLevelNum)
{
    m_iLevelCnt = iLevelNum;
    m_Prototypes = new unordered_map<_wstring, CBase*>[iLevelNum];

    return S_OK;
}

HRESULT CPrototype_Manager::Add_Prototype(_uInt iLevelNum, const _wstring& PrototypeTag, CBase* pPrototype)
{
    if (m_iLevelCnt <= iLevelNum || Find_Prototype(iLevelNum, PrototypeTag))
        return E_FAIL;

    m_Prototypes[iLevelNum].emplace(PrototypeTag, pPrototype);
    return S_OK;
}

void CPrototype_Manager::Clear_Resource(_uInt iLevelIndex)
{
    if (m_iLevelCnt <= iLevelIndex || iLevelIndex < 0)
        return;

    for (auto& pair : m_Prototypes[iLevelIndex])
        Safe_Release(pair.second);

    m_Prototypes[iLevelIndex].clear();
}

CBase* CPrototype_Manager::Find_Prototype(_uInt iLevelNum, const _wstring& PrototypeTag)
{
    if (m_iLevelCnt <= iLevelNum || iLevelNum < 0)
        return nullptr;

    auto pair =  m_Prototypes[iLevelNum].find(PrototypeTag);
    if (pair == m_Prototypes[iLevelNum].end())
        return nullptr;

    return pair->second;
}

CPrototype_Manager* CPrototype_Manager::Create(_uInt iLevelNum)
{
    CPrototype_Manager* pInstance = new CPrototype_Manager();
    if (FAILED(pInstance->Initialize(iLevelNum)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : PROTOTYPE MANAGER");
    }

    return pInstance;
}

void CPrototype_Manager::Free()
{
    for (_uInt i = 0; i < m_iLevelCnt; ++i)
    {
        for (auto pair : m_Prototypes[i])
        {
            Safe_Release(pair.second);
        }
        m_Prototypes[i].clear();
    }
    Safe_Delete_Array(m_Prototypes);
}
