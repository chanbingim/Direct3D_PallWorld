#include "Prototype_Manager.h"

#include "StringHelper.h"

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

CBase* CPrototype_Manager::Clone_Prototype(OBJECT_ID eType, _uInt iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
    CBase* BaseObject = Find_Prototype(iLevelIndex, strPrototypeTag);
    if (nullptr == BaseObject)
        return nullptr;

    switch (eType)
    {
    case OBJECT_ID::GAMEOBJECT :
        return static_cast<CGameObject*>(BaseObject)->Clone(pArg);

    case OBJECT_ID::COMPONENT:
        return static_cast<CComponent*>(BaseObject)->Clone(pArg);
    }

    return nullptr;
}

void CPrototype_Manager::Clear_Resource(_uInt iLevelIndex)
{
    if (m_iLevelCnt <= iLevelIndex || iLevelIndex < 0)
        return;

    for (auto& pair : m_Prototypes[iLevelIndex])
        Safe_Release(pair.second);

    m_Prototypes[iLevelIndex].clear();
}

void CPrototype_Manager::GetPrototypeName(const char* classTypeName, _string& OutName)
{
    WCHAR ClassName[MAX_PATH] = {};
    char  PrototypeName[MAX_PATH] = {};

    CStringHelper::ConvertUTFToWide(classTypeName, ClassName);
    
    for (_uInt i = 0; i < m_iLevelCnt; ++i)
    {
        auto iter = find_if(m_Prototypes[i].begin(), m_Prototypes[i].end(), [&](auto& pair)
            {
                return !strcmp(classTypeName, typeid(*(pair.second)).name());
            });

        if (iter == m_Prototypes[i].end())
            OutName = "";
        else
        {
            CStringHelper::ConvertWideToUTF(iter->first.c_str(), PrototypeName);
            OutName = PrototypeName;
            break;
        }
    }
}

_uInt CPrototype_Manager::GetPrototypeLevel(const _wstring& PrototypeName)
{
    for (_uInt i = 0; i < m_iLevelCnt; ++i)
    {
        auto iter = m_Prototypes[i].find(PrototypeName);
        if (iter != m_Prototypes[i].end())
            return i;
    }
    return -1;
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
        for (auto& pair : m_Prototypes[i])
        {
            Safe_Release(pair.second);
        }
        m_Prototypes[i].clear();
    }
    Safe_Delete_Array(m_Prototypes);
}
