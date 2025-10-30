#include "TerrainManager.h"

#include "GameInstance.h"

#include "PlayerManager.h"
#include "Player.h"

#include "PaseDataHeader.h"
#include "StringHelper.h"

#include "Chunk.h"
#include "FastTravelObject.h"

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

void CTerrainManager::UpdateChunk(_matrix WorldMat)
{
    for (auto pChunk : m_pMapNavigation)
        pChunk.second->GetChunckNavigation()->Update(WorldMat);
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

HRESULT CTerrainManager::ADD_FastTravel(const WCHAR* szMapTag, CFastTravelObject* pFastTravel)
{
    auto iter = m_pMapTransport.find(szMapTag);
    if (iter == m_pMapTransport.end())
    {
        Safe_AddRef(pFastTravel);
        m_pMapTransport.emplace(szMapTag, pFastTravel);
    }
    else
    {
        Safe_Release(pFastTravel);
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CTerrainManager::Remove_FastTravel(const WCHAR* szMapTag)
{
    auto iter = m_pMapTransport.find(szMapTag);
    if (iter == m_pMapTransport.end())
        return E_FAIL;

    Safe_Release(iter->second);
    m_pMapTransport.erase(iter);
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

void CTerrainManager::PlayChunkBGM()
{
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    auto pPlayer = CPlayerManager::GetInstance()->GetCurrentPlayer();
    pGameInstance->Manager_StopSound(CHANNELID::BGM);

    auto ChunkName = pPlayer->GetPlayerOnChunkName();
    
    // 나중에 바꾸자 맵에다가 bgm을 바인딩해서 재생하는걸로
    // 사실 청크별로 맵 사운드 데이터를 가지고있으면 유연한데
    // 영상부터 찍고나서 다시 슛
    if (TEXT("MainArea") == ChunkName)
    {
        pGameInstance->Manager_PlayBGM(TEXT("driftveil-city.mp3"), 0.7f);
    }
    else if (TEXT("PinkCatField") == ChunkName)
    {
        pGameInstance->Manager_PlayBGM(TEXT("Map1Sound.wav"), 0.7f);
    }
    else if (TEXT("SheepBalField") == ChunkName)
    {
        pGameInstance->Manager_PlayBGM(TEXT("BGM_TrainingRoom_01_A.OGG"), 0.7f);
    }
    else if (TEXT("BossField") == ChunkName)
    {
        pGameInstance->Manager_PlayBGM(TEXT("BossSound.wav"), 0.7f);
    }
}

_bool CTerrainManager::ComputeHieght(CTransform* pTransform, _float3* vOutPoint, _bool bIsUpdateCell)
{
    _float3 vPosition = pTransform->GetPosition();

    CHUNK_DESC ChunkData = {};
    Find_Chunk(vPosition, &ChunkData);
    if (nullptr == ChunkData.pChunk)
        return false;

    ChunkData.pChunk->GetChunckNavigation()->ComputeHeight(pTransform, bIsUpdateCell);
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

CChunk* CTerrainManager::Find_ChunkFromTag(const WCHAR* szMapTag)
{
    auto iter = m_pMapNavigation.find(szMapTag);
    if (iter == m_pMapNavigation.end())
        return nullptr;

    return iter->second;
}

_bool CTerrainManager::Find_FastTravelTransport(const WCHAR* szMapTag, _float3* vOut)
{
    auto iter = m_pMapTransport.find(szMapTag);
    if (iter == m_pMapTransport.end())
        return false;

    auto vPosition = iter->second->GetTransform()->GetPosition();
    auto vDir = iter->second->GetTransform()->GetLookVector() * 10.f;
    XMStoreFloat3(vOut, XMLoadFloat3(&vPosition) + vDir);
    return true;
}

void CTerrainManager::Render()
{
    for (auto& pair : m_pMapNavigation)
        pair.second->Render();
}

void CTerrainManager::Render(const WCHAR* szMapName)
{
    auto iter = m_pMapNavigation.find(szMapName);
    if (iter == m_pMapNavigation.end())
        return;

    iter->second->Render();
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

    for (auto& iter : m_pMapTransport)
        Safe_Release(iter.second);
    m_pMapTransport.clear();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
