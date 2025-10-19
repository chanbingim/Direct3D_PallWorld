#include "Npc.h"

#include "GameInstance.h"
#include "PellManager.h"

#include "TerrainManager.h"
#include "Chunk.h"

#pragma region PARTS HEADER
#include "NpcStateMachine.h"
#include "NpcBody.h"
#pragma endregion

#include "GamePlayHUD.h"
#include "DiallogUI.h"

CNpc::CNpc(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CContainerObject(pGraphic_Device, pDeviceContext)
{
}

CNpc::CNpc(const CNpc& rhs) :
    CContainerObject(rhs),
    m_NpcID(rhs.m_NpcID)
{
}

HRESULT CNpc::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CNpc::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(SetUpDefaultNpcData()))
        return E_FAIL;

    return S_OK;
}

void CNpc::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CNpc::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    if (NPC_TEAM::FRENDLY == m_eTeam)
    {
        _vector vCalCamereaPos = m_pGameInstance->GetCameraState(WORLDSTATE::POSITION);
        _vector vCalCamereaLook = m_pGameInstance->GetCameraState(WORLDSTATE::LOOK);
        CCollision::DEFAULT_HIT_DESC	RayHitDesc = {};
        if (m_pCollision->RayHit(vCalCamereaPos, vCalCamereaLook, RayHitDesc))
        {
            if (RayHitDesc.vfDistance < m_fActionDistance)
            {
                if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_F))
                {
                    // 여기서 스크립트 보여주기
                    // 카메라 매니저 보여주고
                    // Npc의 대화 스크립트 로직을 통해서 관리
                    // NPC랑 대화할때 카메라 무브랑 Idle 모션으로 보여주기
                    m_pNpcFsm->ChangeState(TEXT("Dialog_Layer"), TEXT("Talking"));
                    //CGamePlayHUD* pGamePlayHUD = static_cast<CGamePlayHUD*>(m_pGameInstance->GetCurrentHUD());
                    //pGamePlayHUD->ActivePopUpUserInterface(3);
                    //
                    //CDiallogUI* pDiallog = static_cast<CDiallogUI*>(pGamePlayHUD->GetPopUpUserInterface(3));
                    //pDiallog->SetDiallogText(TEXT("NPC와 상호작용 중 테스트 테스트"));
                }
            }
        }
    }
}

void CNpc::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);

    if (!m_IsDead)
        m_pGameInstance->ADD_CollisionList(m_pCollision);
}

HRESULT CNpc::Render()
{
    if (!m_IsDead)
    {
        m_pCollision->Render();
    }

    return S_OK;
}

const _bool CNpc::GetFinisehdAnimation() const
{
    if (nullptr == m_pNpcBody)
        return false;
    return m_pNpcBody->FinishedAnimation();
}

HRESULT CNpc::SetUpDefaultNpcData()
{
    return S_OK;
}

HRESULT CNpc::ADD_PellInfoUI()
{
    return S_OK;
}

_bool CNpc::NpcPlayFSM(_float fDeletaTime)
{
    m_pNpcFsm->Update(fDeletaTime);
    return m_pNpcFsm->GetLayerAnimLoop(TEXT("Dialog_Layer"));
}

void CNpc::SettingNavigation()
{
#pragma region NAVI_MESH
    m_pTerrainManager = CTerrainManager::GetInstance();
    Safe_AddRef(m_pTerrainManager);

    _float3 vPlayPosition = m_pTransformCom->GetPosition();
    CTerrainManager::CHUNK_DESC ChunkDesc = {};
    m_pTerrainManager->Find_Chunk(vPlayPosition, &ChunkDesc);
    if (ChunkDesc.pChunk)
    {
        auto pOrizinNaviMesh = ChunkDesc.pChunk->GetChunckNavigation();
        m_szChunkName = ChunkDesc.ChunkName;

        CNavigation::NAVIGATION_DESC Desc = {};
        Desc.iCurrentCellIndex = pOrizinNaviMesh->Find_Cell(XMLoadFloat3(&vPlayPosition));
        Safe_Release(m_pNevigation);

        m_pNevigation = static_cast<CNavigation*>(pOrizinNaviMesh->Clone(&Desc));
        Safe_AddRef(m_pNevigation);

        auto pair = m_pComponentMap.find(TEXT("NaviMesh_Com"));
        if (pair == m_pComponentMap.end())
        {
            m_pComponentMap.emplace(TEXT("NaviMesh_Com"), m_pNevigation);
        }
        else
        {
            Safe_Release(pair->second);
            pair->second = m_pNevigation;
        }
    }
#pragma endregion
}

void CNpc::ShowActionUI()
{
}

CGameObject* CNpc::Clone(void* pArg)
{
    return nullptr;
}

void CNpc::Free()
{
    __super::Free();

    Safe_Release(m_pNpcFsm);
    Safe_Release(m_pNevigation);
    Safe_Release(m_pCollision);
    Safe_Release(m_pTerrainManager);
}
