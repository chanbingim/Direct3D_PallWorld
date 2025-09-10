#include "TerrainManager.h"

#include "PaseDataHeader.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CTerrainManager);

CTerrainManager::CTerrainManager()
{
}

void CTerrainManager::Initialize(void* pArg)
{
    m_Terrians.resize(10);
}

HRESULT CTerrainManager::LoadTerrianData(const char* pFileData)
{
    // 터레인 파일을 읽어들여 터레인을 세팅한다.
    // 터레인 파일안에 네비메시세팅 파일이 같이 들어있음
    // 아니면 네비메시 파일만 따로읽던가
    ios_base::openmode flag;
    flag = ios::in;

    ifstream file(pFileData, flag);

    vector<SAVE_LEVEL_DESC> ReadFileDesc;
  
    if (file.is_open())
    {
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
            ReadFileDesc.push_back(Desc);
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
