#include "TerrainManager.h"

#include "PaseDataHeader.h"
#include "StringHelper.h"
#include "GameInstance.h"

#include "DefaultMap.h"
#include "BossMap.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CTerrainManager);

CTerrainManager::CTerrainManager()
{
}

void CTerrainManager::Initialize(void* pArg)
{
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    //m_TerrianSize = { 256, 256 };
    //m_Terrians.reserve(10);
    //LoadTerrianData("../Bin/Save/Map/Layer_GamePlay_Terrian.txt");

    auto Object = pGameInstance->GetAllObejctToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Terrian"))->begin();
    m_DefaultMap = static_cast<CDefaultMap*>(*Object);
   // m_DefaultMap = static_cast<CBossMap*>(*Object);
    Safe_AddRef(m_DefaultMap);
}

HRESULT CTerrainManager::LoadTerrianData(const char* pFileData)
{
   

    return S_OK;
}

HRESULT CTerrainManager::CreateTerrian(void* pArg)
{
    SAVE_LEVEL_DESC* Desc = static_cast<SAVE_LEVEL_DESC*>(pArg);


    return S_OK;
}

CNavigation* CTerrainManager::GetNavimesh()
{
    return static_cast<CNavigation*>(m_DefaultMap->Find_Component(TEXT("NaviMesh_Com")));
}

void CTerrainManager::GetAllNaviMeshTriangle(list<NAVI_TRIANGLE>* pOut)
{
    auto pNavi = GetNavimesh();
    *pOut = pNavi->GetNaviMeshTriangleList();
}

void CTerrainManager::Free()
{
    Safe_Release(m_DefaultMap);
}
