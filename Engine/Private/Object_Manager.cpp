#include "Object_Manager.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Layer.h"

CObject_Manager::CObject_Manager() :
   m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CObject_Manager::Initialize(_uInt iLevelCnt)
{
    m_iLevelNum = iLevelCnt;
    m_pLayers = new unordered_map<_wstring, CLayer*>[iLevelCnt];

    return S_OK;
}

void CObject_Manager::Priority_Update(_float fDeletaTime)
{
    for (_uInt i = 0; i < m_iLevelNum; ++i)
    {
        for (auto& pair : m_pLayers[i])
            pair.second->Priority_Update(fDeletaTime);
    }
}

void CObject_Manager::Update(_float fDeletaTime)
{
    for (_uInt i = 0; i < m_iLevelNum; ++i)
    {
        for (auto& pair : m_pLayers[i])
            pair.second->Update(fDeletaTime);
    }
}

void CObject_Manager::Late_Update(_float fDeletaTime)
{
    for (_uInt i = 0; i < m_iLevelNum; ++i)
    {
        for (auto& pair : m_pLayers[i])
            pair.second->Late_Update(fDeletaTime);
    }
}

HRESULT CObject_Manager::Add_GameObject_ToLayer(_uInt iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uInt iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
    auto pGameObject = static_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
    if (nullptr == pGameObject)
        return E_FAIL;

    CLayer* pLayer = Find_Layer(iLayerLevelIndex, strLayerTag);
    if (nullptr == pLayer)
    {
        pLayer = CLayer::Create();

        pLayer->ADD_GameObject(pGameObject);

        m_pLayers[iLayerLevelIndex].emplace(strLayerTag, pLayer);
    }
    else
        pLayer->ADD_GameObject(pGameObject);

    return S_OK;
}

const list<CGameObject*>* CObject_Manager::GetAllObejctToLayer(_uInt iLayerIndex, const _wstring& LayerTag)
{
    auto Layer = Find_Layer(iLayerIndex, LayerTag);
    if (nullptr == Layer)
        return nullptr;

    return Layer->GetAllObjects();
}

void CObject_Manager::Clear_Resource(_uInt iLevelIndex)
{
    if (m_iLevelNum <= iLevelIndex || 0 > iLevelIndex)
        return;

    for (auto& pair : m_pLayers[iLevelIndex])
    {
        pair.second->Clear_Resource();
    }
}

void CObject_Manager::Clear_DeadObject()
{
    for (_uInt i = 0; i < m_iLevelNum; ++i)
    {
        for (auto& Pair : m_pLayers[i])
        {
            Pair.second->Clear_DeadObject();
        }
    }
}

const unordered_map<_wstring, CLayer*>* CObject_Manager::GetLayer(_uInt iLevelID)
{
    if (m_iLevelNum <= iLevelID || 0 > iLevelID)
        return nullptr;

    return &m_pLayers[iLevelID];
}

CLayer* CObject_Manager::Find_Layer(_uInt iLevelIndex, const _wstring LayerTag)
{
    if (m_iLevelNum <= iLevelIndex || 0 > iLevelIndex)
        return nullptr;

    auto pair = m_pLayers[iLevelIndex].find(LayerTag);
    if (pair == m_pLayers[iLevelIndex].end())
        return nullptr;

    return pair->second;
}

CObject_Manager* CObject_Manager::Create(_uInt _iLevelCnt)
{
    CObject_Manager* pObjectManager = new CObject_Manager();
    if (FAILED(pObjectManager->Initialize(_iLevelCnt)))
    {
        Safe_Release(pObjectManager);
        MSG_BOX("CREATE FAIL : OBJECT MANAGER");
    }

    return pObjectManager;
}

void CObject_Manager::Free()
{
    Safe_Release(m_pGameInstance);
    for (_uInt i = 0; i < m_iLevelNum; ++i)
    {
        for (auto& Pair : m_pLayers[i])
        {
            Safe_Release(Pair.second);
        }
        m_pLayers[i].clear();
    }
    Safe_Delete_Array(m_pLayers);
}
