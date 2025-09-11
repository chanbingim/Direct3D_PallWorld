#include "TerrainManager.h"

#include "PaseDataHeader.h"
#include "StringHelper.h"
#include "GameInstance.h"

#include "Terrain.h"

IMPLEMENT_SINGLETON(CTerrainManager);

CTerrainManager::CTerrainManager()
{
}

void CTerrainManager::Initialize(void* pArg)
{
    m_TerrianSize = { 256, 256 };
    m_Terrians.reserve(10);
    LoadTerrianData("../Bin/Save/Map/Layer_GamePlay_Terrian.txt");
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

_bool CTerrainManager::IsMove(_float3 vPosition)
{
     _Int iCellX = (_Int)(vPosition.x / m_TerrianSize.x);
     _Int iCellY = (_Int)(vPosition.z / m_TerrianSize.y);

     if (m_Terrians.size() <= iCellY * m_TerrianSize.x + iCellX)
         return false;

     auto pTerrain = m_Terrians[iCellY * m_TerrianSize.x + iCellX];
     if (nullptr == pTerrain)
         return false;

     _vector vPos = XMLoadFloat3(&vPosition);
    return pTerrain->IsMoveTerrian(vPos);
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

    m_Terrians.push_back(pTerrain);
    return S_OK;
}

HRESULT CTerrainManager::GetCurrentTerrainNaviMesh(_float3 vObejctPos, _uInt iCellIndex, CComponent** ppComponent)
{
    _Int iCellX = (_Int)(vObejctPos.x / m_TerrianSize.x);
    _Int iCellY = (_Int)(vObejctPos.z / m_TerrianSize.y);
    if (m_Terrians.size() <= iCellY * m_TerrianSize.x + iCellX)
        return E_FAIL;

    auto pTerrain = m_Terrians[iCellY * m_TerrianSize.x + iCellX];
    if (nullptr == pTerrain)
        return E_FAIL;

    auto pNaviMesh = pTerrain->Find_Component(TEXT("NaviMesh_Com"));
    if (nullptr == pNaviMesh)
        return E_FAIL;
    
    CNavigation::NAVIGATION_DESC NaviDesc = {};
    NaviDesc.iCurrentCellIndex = iCellIndex;
    *ppComponent = pNaviMesh->Clone(&NaviDesc);
    return S_OK;
}

HRESULT CTerrainManager::ADD_Terrian(CTerrain* pTerrian)
{
    if (nullptr == pTerrian)
        return E_FAIL;

    m_Terrians.push_back(pTerrian);
    return S_OK;
}

void CTerrainManager::Free()
{
    for (auto& iter : m_Terrians)
        Safe_Release(iter);
}
