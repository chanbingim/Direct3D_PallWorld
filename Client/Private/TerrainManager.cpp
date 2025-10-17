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

void CTerrainManager::Initialize(void* pArg)
{
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    //m_TerrianSize = { 256, 256 };
    //m_Terrians.reserve(10);
    //LoadTerrianData("../Bin/Save/Map/Layer_GamePlay_Terrian.txt");

    auto Object = pGameInstance->GetAllObejctToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Terrian"))->begin();
    m_DefaultMap = static_cast<CDefaultMap*>(*Object);
    Safe_AddRef(m_DefaultMap);
}

HRESULT CTerrainManager::LoadTerrianData(const char* pFileData)
{
    // 터레인 파일을 읽어들여 터레인을 세팅한다.
    // 터레인 파일안에 네비메시세팅 파일이 같이 들어있음
    // 아니면 네비메시 파일만 따로읽던가
    ios_base::openmode flag;
    flag = ios::in;

    ifstream file(pFileData, flag);

    _uInt iNumTerrianCnt = {};
    if (file.is_open())
    {
        file >> iNumTerrianCnt;
        while (file)
        {
            //터레인 값 읽어오기
            SAVE_LEVEL_DESC Desc = {};
            file >> Desc.eType >> Desc.iPrototypeLevelID >> Desc.iSaveLevelID;
            file >> Desc.PrototypeName >> Desc.LayerName >> Desc.PrototypeIndex;
            file >> Desc.vScale.x >> Desc.vScale.y >> Desc.vScale.z;
            file >> Desc.vRotation.x >> Desc.vRotation.y >> Desc.vRotation.z;
            file >> Desc.vPosition.x >> Desc.vPosition.y >> Desc.vPosition.z;
            file >> Desc.ObjectDesc.TerrianDesc.TerrainType;
            file >> Desc.ObjectDesc.TerrianDesc.TileCnt.x >> Desc.ObjectDesc.TerrianDesc.TileCnt.y;
            file >> Desc.ObjectDesc.TerrianDesc.HeightMapCom;
            file >> Desc.ObjectDesc.TerrianDesc.NaviMeshPath;
         
            if (FAILED(CreateTerrian(&Desc)))
                break;
        }
    }
    file.close();

    return S_OK;
}

HRESULT CTerrainManager::CreateTerrian(void* pArg)
{
    SAVE_LEVEL_DESC* Desc = static_cast<SAVE_LEVEL_DESC*>(pArg);

    WCHAR   ProtoTypeName[MAX_PATH], LayerName[MAX_PATH];
    CGameInstance* pGameInstance = CGameInstance::GetInstance();

    CTerrain::TERRIAN_DESC ObjectDesc = {};
    lstrcpy(ObjectDesc.ObjectTag, L"Terrian");
    CStringHelper::ConvertUTFToWide(Desc->PrototypeName, ProtoTypeName);
    CStringHelper::ConvertUTFToWide(Desc->LayerName, LayerName);
    ObjectDesc.vScale = Desc->vScale;
    ObjectDesc.vRotation = Desc->vRotation;
    ObjectDesc.vPosition = Desc->vPosition;
    ObjectDesc.TerrianType = Desc->ObjectDesc.TerrianDesc.TerrainType;
    ObjectDesc.iGridCnt = Desc->ObjectDesc.TerrianDesc.TileCnt.x;
    CStringHelper::ConvertUTFToWide(Desc->ObjectDesc.TerrianDesc.HeightMapCom, ObjectDesc.HeightMap);
    CStringHelper::ConvertUTFToWide(Desc->ObjectDesc.TerrianDesc.NaviMeshPath, ObjectDesc.NavigationData);

    auto pTerrain = static_cast<CTerrain*>(pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, Desc->iPrototypeLevelID, ProtoTypeName, &ObjectDesc));
    if (nullptr == pTerrain)
        return E_FAIL;

    pGameInstance->Add_GameObject_ToLayer(Desc->iSaveLevelID, LayerName, pTerrain);
    Safe_AddRef(pTerrain);

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
