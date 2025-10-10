#include "PellBase.h"
#include "GameInstance.h"
#include "PellManager.h"

#pragma region PARTS HEADER
#include "PellStateMachine.h"
#include "NeturalPellInfo.h"
#include "PellBody.h"
#include "Recovery.h"
#pragma endregion

#pragma region STATE
#include "PellPatrolState.h"
#pragma endregion

#pragma region PLAYER
#include "PlayerManager.h"
#include "PlayerStateMachine.h"
#include "Player.h"
#pragma endregion

#include "PalSpher.h"
#include "ProjectileObject.h"
#include "CombatComponent.h"

CPellBase::CPellBase(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CContainerObject(pGraphic_Device, pDeviceContext)
{
   
}

CPellBase::CPellBase(const CPellBase& rhs) :
    CContainerObject(rhs),
    m_PellID(rhs.m_PellID)
{
}

HRESULT CPellBase::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPellBase::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(SetUpDefaultPellData()))
        return E_FAIL;

    m_PellInfo.iLevel = 1;
    m_PellInfo.ePellStorageState = PELL_STORAGE_STATE::WORLD;
    return S_OK;
}

void CPellBase::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);

    if (VISIBILITY::VISIBLE == m_pNeturalPellUI->GetVisibility())
        m_pNeturalPellUI->Priority_Update(fDeletaTime);
   
}

void CPellBase::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    if (VISIBILITY::VISIBLE == m_pNeturalPellUI->GetVisibility())
        m_pNeturalPellUI->Update(fDeletaTime);
}

void CPellBase::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);

    if (VISIBILITY::VISIBLE == m_pNeturalPellUI->GetVisibility())
        m_pNeturalPellUI->Late_Update(fDeletaTime);

    if (!m_IsDead)
    {
        m_pGameInstance->ADD_CollisionList(m_pCollision);
    }
}

HRESULT CPellBase::Render()
{
    if (!m_IsDead)
    {
        m_pCollision->Render();
    }
      
    return S_OK;
}

const _bool CPellBase::GetFinisehdAnimation() const
{
    if (nullptr == m_pPellBody)
        return false;
    return m_pPellBody->FinishedAnimation();
}

void CPellBase::Damage(void* pArg, CActor* pDamagedActor)
{
    if (nullptr == pArg)
    {
        auto HitPalSpher = dynamic_cast<CPalSpher*>(pDamagedActor);
        if (nullptr != HitPalSpher)
        {
            switch (m_PellInfo.ePellStorageState)
            {
            case PELL_STORAGE_STATE::WORLD:
                m_PellInfo.ePellStorageState = PELL_STORAGE_STATE::PELL_SHPER;
                break;
            }
        }
    }
    else
    {
        if (nullptr == pDamagedActor)
            return;

        _bool bIsAddTarget = false;
        DEFAULT_DAMAGE_DESC* DamageDesc = static_cast<DEFAULT_DAMAGE_DESC*>(pArg);

        auto pProjectileHit = dynamic_cast<CProjectileObject *>(pDamagedActor);
        if (pProjectileHit)
        {
            pDamagedActor = nullptr;
        }
        else
        {
            bIsAddTarget = true;
        }

        if (DamageDesc)
        {
            m_PellInfo.CurHealth -= DamageDesc->fDmaged;
            if (0 >= m_PellInfo.CurHealth)
            {
                m_bIsLoop = false;
                m_pPellFsm->ChangeState(TEXT("CombatLayer"), TEXT("Dead"));
            }
            else
            {
                if (m_pPellBody)
                    m_pPellBody->ResetPellCurrentAnimation();

                if (m_pCombatCom)
                    m_pCombatCom->ADD_TargetObject(pDamagedActor);

                m_bIsAction = true;
                m_bIsLoop = false;
                m_pPellFsm->SetCombatAction(true);
                m_pPellFsm->ChangeState(TEXT("CombatLayer"), TEXT("Hit"));
            }
        }
    }
}

void CPellBase::ChangePellTeam(PELL_TEAM eTeam)
{
    m_eTeam = eTeam;

    m_pPellFsm->ResetLayer(TEXT("CombatLayer"));
    m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Idle"));
    m_pCombatCom->ResetCombatComponent();

    switch (m_eTeam)
    {
    case Client::CPellBase::PELL_TEAM::FRENDLY:
        m_PellInfo.ePellStorageState = PELL_STORAGE_STATE::PLAYER_INVEN;
        break;
    case Client::CPellBase::PELL_TEAM::NEUTRAL:
        m_PellInfo.ePellStorageState = PELL_STORAGE_STATE::WORLD;
        break;
    case Client::CPellBase::PELL_TEAM::ENEMY:

        break;
    }
}

void CPellBase::SpawnPellFriendly()
{
    if (PELL_STORAGE_STATE::PLAYER_INVEN == m_PellInfo.ePellStorageState)
    {
        m_PellInfo.ePellStorageState = PELL_STORAGE_STATE::PARTNER_PELL;
        auto pPlayer = CPlayerManager::GetInstance()->GetCurrentPlayer();
        m_pNevigation->ChangeNaviMeshIndex(pPlayer->GetNaviMeshCell());

        _float3 vPlayerPos = {};
        XMStoreFloat3(&vPlayerPos, m_pGameInstance->GetPlayerState(WORLDSTATE::POSITION));
        m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Idle"));
        m_pPellFsm->CombatStateReset();
        m_pPellFsm->SetCombatAction(false);
        m_pTransformCom->SetPosition(vPlayerPos);
    }
    else if (PELL_STORAGE_STATE::PARTNER_PELL == m_PellInfo.ePellStorageState)
        m_PellInfo.ePellStorageState = PELL_STORAGE_STATE::PLAYER_INVEN;
}

HRESULT CPellBase::SetUpDefaultPellData()
{
    auto DefaultData = CPellManager::GetInstance()->FindPellData(m_PellID);
    if (nullptr != DefaultData)
    {
        m_PellInfo = *DefaultData;
        m_PellInfo.CurHealth = m_PellInfo.MaxHealth;
        m_PellInfo.CurHunger = m_PellInfo.MaxHunger;
        m_PellInfo.CurStemina = m_PellInfo.MaxStemina;
    }

    return S_OK;
}

void CPellBase::CombatAction(_float fDeletaTime, CGameObject* pTarget)
{
}

HRESULT CPellBase::ADD_PellInfoUI()
{
    CNeturalPellInfo::NETURAL_PELL_DESC PellInfoDesc = {};
    PellInfoDesc.pOwner = this;
    PellInfoDesc.vScale = { 2.5f, 0.5f, 0.f };

    _float3 vParentScale = m_pPellBody->GetTransform()->GetScale();
    PellInfoDesc.vPosition = { vParentScale.x * 0.5f +  PellInfoDesc.vScale.x * 0.5f, 
                               vParentScale.y * 0.5f + 3.f , 0.f };

    m_pNeturalPellUI = static_cast<CNeturalPellInfo*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PellInfo_UI"), &PellInfoDesc));
    return S_OK;
}

_bool CPellBase::PellPlayFSM(_float fDeletaTime)
{
    const CPellStateMachine::PELL_STATE& State = m_pPellFsm->GetState();
    if (PELL_STORAGE_STATE::PARTNER_PELL < m_PellInfo.ePellStorageState)
        return false;

    m_fAccActionTime += fDeletaTime;

    if (m_fAccActionTime >= m_PellInfo.fPellActTime)
    {
        PellChiceAction(fDeletaTime);
        m_fAccActionTime = 0.f;
    }
    else if(false == m_IsDead)
    {
        if (PELL_TEAM::NEUTRAL == m_eTeam)
            ShowPellInfo();

        if(m_bIsAction || PELL_STORAGE_STATE::PARTNER_PELL == m_PellInfo.ePellStorageState)
            PellTackingAction(fDeletaTime);

        if (State.bIsCombat)
        {
            if (CPellStateMachine::COMBAT_ACTION::END == State.eCombat_State)
            {
                CGameObject* vTargetObject = m_pCombatCom->GetCurrentTarget();
                if (nullptr != vTargetObject)
                {
                    _float3 vPellPos = m_pTransformCom->GetPosition();
                    _float3 vTargetPos = vTargetObject->GetTransform()->GetPosition();
                    _float Distance = XMVectorGetX((XMVector3Length(XMLoadFloat3(&vTargetPos) - XMLoadFloat3(&vPellPos))));
                    if (false == State.bIsAttacking)
                    {
                        if (5.f <= Distance)
                        {
                            _vector vDirDiff = XMLoadFloat3(&vTargetPos) - XMLoadFloat3(&vPellPos);
                            _float3 vTargetPoint = {};
                            XMStoreFloat3(&vTargetPoint, XMLoadFloat3(&vPellPos) + +vDirDiff * Distance);
                            StartMoveAction(vTargetPoint);
                        }
                    }
                }
            }
        }
    }

    m_pPellFsm->Update(fDeletaTime);
    if(CPellStateMachine::COMBAT_ACTION::END == State.eCombat_State)
        m_bIsLoop = m_pPellFsm->GetLayerAnimLoop(TEXT("BodyLayer"));
    else
        m_bIsLoop = m_pPellFsm->GetLayerAnimLoop(TEXT("CombatLayer"));

    return true;
}

void CPellBase::PellChiceAction(_float fDeletaTime)
{
    switch (m_eTeam)
    {
    case PELL_TEAM::FRENDLY:
        ActionFrendly(fDeletaTime);
        break;
    case PELL_TEAM::NEUTRAL:
        ActionNeutral(fDeletaTime);
        break;
    case PELL_TEAM::ENEMY:
        ActionEnemy(fDeletaTime);
        break;
    }
}

void CPellBase::PellTackingAction(_float fDeletaTime)
{
    const CPellStateMachine::PELL_STATE& State = m_pPellFsm->GetState();
    if (CPellStateMachine::COMBAT_ACTION::DEAD == State.eCombat_State)
    {
        if (PELL_STORAGE_STATE::PARTNER_PELL > m_PellInfo.ePellStorageState)
            DeadNeutalPell();
    }
    else
    {
        if (CPellStateMachine::COMBAT_ACTION::STUN >= State.eCombat_State)
        {
            if (m_pPellFsm->GetLayerLastPhase(TEXT("CombatLayer")) && m_pPellBody->FinishedAnimation())
            {
                m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Idle"));
                m_pPellFsm->CombatStateReset();

                if (State.bIsAttacking)
                    m_pPellFsm->SetAttack(false);

                m_bIsAction = false;
            }
        }
        else
        {
            switch (State.eMove_State)
            {
            case CPellStateMachine::MOVE_ACTION::DEFAULT:
            {
                if (PELL_STORAGE_STATE::PARTNER_PELL == m_PellInfo.ePellStorageState)
                {
                    _float3 vPellPos = m_pTransformCom->GetPosition();
                    _vector vPlayerPos = m_pGameInstance->GetPlayerState(WORLDSTATE::POSITION);
                    _vector vCalulationPellPos = XMLoadFloat3(&vPellPos);

                    _float Distance = XMVectorGetX((XMVector3Length(vPlayerPos - vCalulationPellPos)));
                    if (Distance > 5.f)
                    {
                        //플레이어를 따라다니는 파트너 펠은 플레이어의 이동상태와 상태를 공유해야한다.
                        auto pPlayer = CPlayerManager::GetInstance()->GetCurrentPlayer();
                        CPlayerStateMachine::PLAYER_STATE PlayerState = {};
                        pPlayer->GetPlayerState(&PlayerState);

                        CChaseComponent::CHASE_DESC ChaseDesc = {};
                        ChaseDesc.pTargetTransform = pPlayer->GetTransform();
                        ChaseDesc.fChaseSpeed = &m_fPellMoveSpeed;
                        m_pChase->SetChase(ChaseDesc);

                        CPellPatrolState::PELL_PATROL_STATE_DESC PatrolDesc = {};
                        PatrolDesc.pActPell = this;
                        PatrolDesc.bIsPartnerPell = true;
                        PatrolDesc.pPlayer = pPlayer;
                        PatrolDesc.fPellMoveSpeed = &m_fPellMoveSpeed;

                        m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Patrol"), &PatrolDesc);
                    }
                }
            }
            break;
            case CPellStateMachine::MOVE_ACTION::PATROL:
            {
                if (PELL_STORAGE_STATE::PARTNER_PELL == m_PellInfo.ePellStorageState)
                {
                    _float3 vPellPos = m_pTransformCom->GetPosition();
                    _vector vPlayerPos = m_pGameInstance->GetPlayerState(WORLDSTATE::POSITION);
                    _vector vCalulationPellPos = XMLoadFloat3(&vPellPos);

                    _float Distance = XMVectorGetX((XMVector3Length(vPlayerPos - vCalulationPellPos)));
                    if (5.f >= Distance)
                    {
                        m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Idle"));
                    }
                    else
                    {
                        _float3 vDir{}, vOutMovePoint{};
                        if (0 != m_fPellMoveSpeed)
                        {
                            m_pChase->ComputeLerpPoint(fDeletaTime, vDir, vOutMovePoint);
                            _vector vMoveDistance = XMLoadFloat3(&vOutMovePoint);

                            if (m_pNevigation->IsMove(vMoveDistance + vCalulationPellPos))
                            {
                                m_pTransformCom->LerpTurn({ 0.f, 1.f, 0.f, 0.f }, vMoveDistance + vCalulationPellPos, XMConvertToRadians(180.f), fDeletaTime);
                                m_pTransformCom->ADD_Position(vMoveDistance);
                            }
                        }
                    }
                }
                else
                {
                    // 이거는 여기에서 목표지점에 가까이가거나하면 모드 전환
                    // 이거도 추적이라는 컴포넌트로 관리할거임
                    _vector vTargetPos = XMLoadFloat3(&m_vTargetPoint);
                    _float3 vCurPos = m_pTransformCom->GetPosition();
                    vCurPos.y = 0;
                    _vector vPos = XMLoadFloat3(&vCurPos);
                    if (1 > XMVectorGetX(XMVector3Length(vTargetPos - vPos)))
                    {
                        if (m_PathFinding.empty())
                        {
                            m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Idle"));
                            if (CPellStateMachine::COMBAT_ACTION::END == State.eCombat_State)
                                m_bIsAction = false;
                        }
                        else
                        {
                            auto iter = m_PathFinding.begin();
                            m_vTargetPoint = *iter;
                            m_PathFinding.erase(iter);
                        }
                    }
                }
            }
            }
        }
    }
}

void CPellBase::StartMoveAction(const _float3 vEndPos)
{
    m_PathFinding.clear();
    m_pNevigation->ComputePathfindingAStar(m_pTransformCom->GetPosition(), vEndPos, &m_PathFinding);

    if (!m_PathFinding.empty())
    {
        auto iter = m_PathFinding.begin();
        m_vTargetPoint = *iter;
        m_PathFinding.erase(iter);
        m_bIsAction = true;

        CPellPatrolState::PELL_PATROL_STATE_DESC PatrolDesc = {};
        PatrolDesc.pActPell = this;
        PatrolDesc.bIsCombat = m_pPellFsm->GetState().bIsCombat;
        PatrolDesc.fPellMoveSpeed = &m_fPellMoveSpeed;
        m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Patrol"), &PatrolDesc);
    }
}

void CPellBase::ActionFrendly(_float fDeletaTime)
{
    const CPellStateMachine::PELL_STATE& State = m_pPellFsm->GetState();
    if (false == State.bIsCombat)
    {
        // 전투 중이 아니라면 펠은 플레이어와 일정거리이상 멀어지면 이동한다.
        // 플레이어가 달리기 중이라면 펠도 뛰고
        // 플레이어가 걷기 중이라면 펠도 걷는다.

         //기본적으로 플레이어를 따라다니게 변경
        if (PELL_STORAGE_STATE::WORLD == m_PellInfo.ePellStorageState)
        {
            //여기서 작업 및 이동 로직을 구현해야함
            _float3 vEndPoint = m_pTransformCom->GetPosition();
            vEndPoint.x += m_pGameInstance->Random(-10.f, 10.f);
            vEndPoint.z += m_pGameInstance->Random(-10.f, 10.f);

            StartMoveAction(vEndPoint);
        }
    }
    else
    {
        if (CPellStateMachine::COMBAT_ACTION::DEAD == State.eCombat_State)
            DeadNeutalPell();
        else
        {
            if (!State.bIsAttacking)
            {
                m_pCombatCom->UpdateTarget();
                m_pCombatCom->Update(fDeletaTime);
            }
        }
    }
}

void CPellBase::ActionNeutral(_float fDeletaTime)
{
    const CPellStateMachine::PELL_STATE& State = m_pPellFsm->GetState();
    if (false == State.bIsCombat)
    {
        if (m_bIsAction)
        {
            switch (State.eMove_State)
            {
            case CPellStateMachine::MOVE_ACTION::RESET :
            {
                if (m_PellInfo.MaxStemina <= m_PellInfo.CurStemina)
                {
                    m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Idle"));
                    m_pRecovery->SetRecovery(false);
                    m_bIsAction = false;
                }
            }
            break;
            }
        }
        else
        {
            // 여기서 현재 상태가 전투중이 아니라면
            // 이동 휴식 중에 하나 골라서 진행함
            if (m_PellInfo.MaxStemina * 0.3f >= m_PellInfo.CurStemina)
            {
                if (m_pRecovery)
                {
                    m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Rest"));
                    m_pRecovery->SetRecovery(true);
                    m_bIsAction = true;
                }
            }
            else
            {
                _float3 vEndPoint = m_pTransformCom->GetPosition();
                vEndPoint.x += m_pGameInstance->Random(-5.f, 5.f);
                vEndPoint.z += m_pGameInstance->Random(-5.f, 5.f);

                StartMoveAction(vEndPoint);
            }
        }
    }
    else
    {
        if(CPellStateMachine::COMBAT_ACTION::END == State.eCombat_State)
        {
            if (!State.bIsAttacking)
            {
                m_pCombatCom->UpdateTarget();
                m_pCombatCom->Update(fDeletaTime);
            }
        }
    }
}

void CPellBase::ActionEnemy(_float fDeletaTime)
{
   
}

void CPellBase::ShowPellInfo()
{
    _float3 vCurPos = m_pTransformCom->GetPosition();
    _vector vPlayerPos = m_pGameInstance->GetPlayerState(WORLDSTATE::POSITION);
    _vector vPos = XMLoadFloat3(&vCurPos);
    if (m_fInfoVisibleDistance >= XMVectorGetX(XMVector3Length(vPlayerPos - vPos)))
        m_pNeturalPellUI->SetVisibility(VISIBILITY::VISIBLE);
    else
        m_pNeturalPellUI->SetVisibility(VISIBILITY::HIDDEN);

}

void CPellBase::DeadNeutalPell()
{
    _uInt iPhase = m_pPellFsm->GetStatePhase(TEXT("CombatLayer"));
    switch (iPhase)
    {
    case 0:
        if (m_pPellBody->FinishedAnimation())
        {
            m_pPellFsm->NextStatePhase(TEXT("CombatLayer"));
            m_bIsLoop = true;
            m_fAccActionTime = 0.0f;
        }
        break;
    case 1:
        //여기서 나중에  죽음 이펙트 실행하고 그다음 true로 만들어서 삭제
        m_IsDead = true;
        break;
    }
}

CGameObject* CPellBase::Clone(void* pArg)
{
    return nullptr;
}

void CPellBase::Free()
{
    __super::Free();

    Safe_Release(m_pPellFsm);
    Safe_Release(m_pRecovery);
    Safe_Release(m_pChase);
    Safe_Release(m_pNeturalPellUI);
    Safe_Release(m_pNevigation);
    Safe_Release(m_pCombatCom);
    Safe_Release(m_pCollision);
}
