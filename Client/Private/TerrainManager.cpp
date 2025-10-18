#include "TerrainManager.h"

#include "PaseDataHeader.h"
#include "StringHelper.h"
#include "GameInstance.h"

#include "DefaultMap.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CTerrainManager);

CTerrainManager::CTerrainManager()
{
}

void CTerrainManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);

    auto Object = pGameInstance->GetAllObejctToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Terrian"))->begin();
    m_DefaultMap = static_cast<CDefaultMap*>(*Object);
    Safe_AddRef(m_DefaultMap);
}

CNavigation* CTerrainManager::GetNavimesh()
{
    return static_cast<CNavigation*>(m_DefaultMap->Find_Component(TEXT("NaviMesh_Com")));
}

HRESULT CTerrainManager::ADD_Navigation(const WCHAR* szMapTag, const char* pFilePath)
{
    auto iter = m_pMapNavigation.find(szMapTag);
    if (iter == m_pMapNavigation.end())
    {
        auto pNavigation = CNavigation::Create(m_pDevice, m_pContext, pFilePath);
        if (nullptr == pNavigation)
            return E_FAIL;

        m_pMapNavigation.emplace(szMapTag, pNavigation);
    }
    else
        return E_FAIL;

    return S_OK;
}

HRESULT CTerrainManager::Remove_Navigation(const WCHAR* szMapTag)
{
    auto iter = m_pMapNavigation.find(szMapTag);
    if (iter == m_pMapNavigation.end())
            return E_FAIL;
    
    Safe_Release(iter->second);
    m_pMapNavigation.erase(iter);
    return S_OK;
}

CNavigation* CTerrainManager::Find_MapNavigation(const WCHAR* szMapTag)
{
    auto iter = m_pMapNavigation.find(szMapTag);
    if (iter == m_pMapNavigation.end())
        return nullptr;

    return iter->second;
}

void CTerrainManager::Free()
{
    for (auto& iter : m_pMapNavigation)
        Safe_Release(iter.second);
    m_pMapNavigation.clear();

    Safe_Release(m_DefaultMap);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
