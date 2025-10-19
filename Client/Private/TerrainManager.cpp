#include "TerrainManager.h"

#include "GameInstance.h"
#include "PaseDataHeader.h"
#include "StringHelper.h"

#include "Chunk.h"

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
}
HRESULT CTerrainManager::ADD_Chunk(const WCHAR* szMapTag, void* pArg)
{
    auto iter = m_pMapNavigation.find(szMapTag);
    if (iter == m_pMapNavigation.end())
    {
        auto pNavigation = CChunk::Create(m_pDevice, m_pContext, pArg);
        if (nullptr == pNavigation)
            return E_FAIL;

        m_pMapNavigation.emplace(szMapTag, pNavigation);
    }
    else
        return E_FAIL;

    return S_OK;
}

HRESULT CTerrainManager::Remove_Chunk(const WCHAR* szMapTag)
{
    auto iter = m_pMapNavigation.find(szMapTag);
    if (iter == m_pMapNavigation.end())
            return E_FAIL;
    
    Safe_Release(iter->second);
    m_pMapNavigation.erase(iter);
    return S_OK;
}

_bool CTerrainManager::UpdateChunk(const WCHAR* ChunkKey, _float3 vMovePoint)
{
    auto pair = m_pMapNavigation.find(ChunkKey);
    if (pair == m_pMapNavigation.end())
        return false;

    if (false == m_pMapNavigation[ChunkKey]->IsIn(vMovePoint))
    {
        // 재갱신 여기서도 false 가 나올수도 있음 그러면
        // 못움직이는게 맞음
        return true;
    }

    return false;
}

_bool CTerrainManager::ComputeHieght(CTransform* pTransform, _float3* vOutPoint, _bool bIsUpdateCell)
{
    _float3 vPosition = pTransform->GetPosition();

    CHUNK_DESC ChunkData = {};
    Find_Chunk(vPosition, &ChunkData);
    if (nullptr == ChunkData.pChunk)
        return false;

    if (bIsUpdateCell)
    {
        _uInt iCurrentCell = ChunkData.pChunk->GetChunckNavigation()->Find_Cell(XMLoadFloat3(&vPosition));
        ChunkData.pChunk->GetChunckNavigation()->ChangeNaviMeshIndex(iCurrentCell);
    }

    (*vOutPoint).y = ChunkData.pChunk->GetChunckNavigation()->ComputeHeight(pTransform);
    return true;
}

void CTerrainManager::Find_Chunk(_float3 vPos, CHUNK_DESC* pOutDesc)
{
    pOutDesc->ChunkName = TEXT("");
    pOutDesc->pChunk = nullptr;

    for (auto& pair : m_pMapNavigation)
    {
        if (pair.second->IsIn(vPos))
        {
            pOutDesc->ChunkName = pair.first;
            pOutDesc->pChunk = pair.second;
            break;
        }
    }
}

CChunk* CTerrainManager::Find_MapNavigation(const WCHAR* szMapTag)
{
    auto pair = m_pMapNavigation.find(szMapTag);
    if (pair == m_pMapNavigation.end())
        return nullptr;

    return pair->second;
}

void CTerrainManager::Free()
{
    for (auto& iter : m_pMapNavigation)
        Safe_Release(iter.second);
    m_pMapNavigation.clear();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
