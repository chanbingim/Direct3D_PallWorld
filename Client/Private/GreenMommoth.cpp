#include "GreenMommoth.h"
#include "GameInstance.h"

#include "TerrainManager.h"
#include "GamePlayHUD.h"
#include "Level.h"

#pragma region Client Compoent
#include "CombatComponent.h"
#include "DefenceBossComponent.h"
#include "AiSenceComponent.h"
#pragma endregion

#include "PellStateMachine.h"
#include "PellAttackState.h"
#include "PellPatrolState.h"
#include "PellSkillManager.h"

#include "FastTravelObject.h"
#include "PellBody.h"

CGreenMommoth::CGreenMommoth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPellBase(pDevice, pContext)
{
}

CGreenMommoth::CGreenMommoth(const CGreenMommoth& rhs) :
    CPellBase(rhs)
{
    m_PellID = 3;
}

HRESULT CGreenMommoth::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CGreenMommoth::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_PellFsm()))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_PartObjects()))
        return E_FAIL;

    m_eTeam = ACTOR_TEAM::BOSS;

    m_PellInfo.PartnerSkillList.push_back(*CPellSkillManager::GetInstance()->FindPellData(5));
    return S_OK;
}

void CGreenMommoth::Priority_Update(_float fDeletaTime)
{
    CContainerObject::Priority_Update(fDeletaTime);
    auto PellState = m_pPellFsm->GetState();


    m_pCombatCom->UpdateTarget();
    m_pPellFsm->Update(fDeletaTime);
    auto pTarget = m_pCombatCom->GetCurrentTarget();
    if(pTarget)
    {
        _float3 vTargetPos = pTarget->GetTransform()->GetPosition();
        _vector vCalTargetPos = XMLoadFloat3(&vTargetPos);
        m_pTransformCom->LerpTurn(m_pTransformCom->GetUpVector(), vCalTargetPos, XMConvertToRadians(180.f), fDeletaTime);

        if (false == m_pPellFsm->GetState().bIsAttacking)
        {
            m_pCombatCom->Update(fDeletaTime);
            m_pPellFsm->SetCombatAction(true);
        }
        else
        {
            // 여기서 공격중이라면 공격 애니메이션이 끝나면 다시 Idle
            if (m_pPellFsm->GetLayerLastPhase(TEXT("CombatLayer")) && m_pPellBody->FinishedAnimation())
            {
                switch (PellState.eCombat_State)
                {
                case CPellStateMachine::COMBAT_ACTION::ATTACK:
                case CPellStateMachine::COMBAT_ACTION::STUN:
                case CPellStateMachine::COMBAT_ACTION::HIT:
                    m_pPellFsm->CombatStateReset();
                    m_pPellFsm->ChangeState(TEXT("Body_Layer"), TEXT("Idle"));
                    break;
                case CPellStateMachine::COMBAT_ACTION::DEAD :
                    SetDead(true);
                    break;
                }
            }
        }
    }

    m_pNevigation->ComputeHeight(m_pTransformCom, false);
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

void CGreenMommoth::Update(_float fDeletaTime)
{
    _bool      bIsAnimLoop = {};
    if(CPellStateMachine::COMBAT_ACTION::END != m_pPellFsm->GetState().eCombat_State)
        bIsAnimLoop = m_pPellFsm->GetLayerAnimLoop(TEXT("CombatLayer"));
    else
        bIsAnimLoop = m_pPellFsm->GetLayerAnimLoop(TEXT("Body_Layer"));
   
    m_pPellBody->PellPlayAnimation(m_pPellFsm->GetAnimationName().c_str(), bIsAnimLoop);
    m_pAiSenceCom->UpdatSenceComponent(fDeletaTime);
    CContainerObject::Update(fDeletaTime);
}

void CGreenMommoth::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        CContainerObject::Late_Update(fDeletaTime);
        m_pGameInstance->ADD_CollisionList(m_pCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CGreenMommoth::Render()
{
    __super::Render();
    return S_OK;
}

void CGreenMommoth::Damage(void* pArg, CActor* pDamagedActor)
{
    __super::Damage(pArg, pDamagedActor);
}

void CGreenMommoth::CombatAction(_float fDeletaTime, CGameObject* pTarget)
{
    auto PellState = m_pPellFsm->GetState();

    _float3 vTargetPos = pTarget->GetTransform()->GetPosition();
    _float3 vPos = m_pTransformCom->GetPosition();

    _float fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vTargetPos) - XMLoadFloat3(&vPos)));

    if (m_PellInfo.fPellAttackRange >= fDistance)
    {
        CPellAttackState::PELL_ATTACK_STATE_DESC AttackDesc = {};
        AttackDesc.ActPell = this;
        AttackDesc.fSkillMoveSpeed = &m_fPellMoveSpeed;
        AttackDesc.IsSpaceOut = false;
        m_bIsAction = true;
        if (fDistance < 15.f)
            AttackDesc.AttackData = &m_PellInfo.DefaultSkill;
        else
            AttackDesc.AttackData = &m_PellInfo.PartnerSkillList[0];
        

        m_pPellFsm->ChangeState(TEXT("CombatLayer"), TEXT("Attack"), &AttackDesc);
        m_pPellFsm->SetAttack(true);
    }
    else if(PellState.bIsCombat)
    {
        switch (PellState.eMove_State)
        {
        case CPellStateMachine::MOVE_ACTION::DEFAULT :
        {
            CChaseComponent::CHASE_DESC ChaseDesc = {};
            ChaseDesc.pTargetTransform = pTarget->GetTransform();
            ChaseDesc.fChaseSpeed = &m_fPellMoveSpeed;
            m_pChase->SetChase(ChaseDesc);

            CPellPatrolState::PELL_PATROL_STATE_DESC PatrolDesc = {};
            PatrolDesc.pActPell = this;
            PatrolDesc.bIsPartnerPell = false;
            PatrolDesc.pPlayer = nullptr;
            PatrolDesc.fPellMoveSpeed = &m_fPellMoveSpeed;

            m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Patrol"), &PatrolDesc);
        }
        break;
        case CPellStateMachine::MOVE_ACTION::PATROL :
        {
            _float3 vDir{}, vOutMovePoint{};
            if (0 != m_fPellMoveSpeed)
            {
                _float3 vPellPos = m_pTransformCom->GetPosition();
                _vector vCalTargetPos = XMLoadFloat3(&vTargetPos);
                _vector vCalPellPos = XMLoadFloat3(&vPellPos);

                m_pChase->ComputeLerpPoint(fDeletaTime, vDir, vOutMovePoint);
                _vector vMoveDistance = XMLoadFloat3(&vOutMovePoint);

                if (m_pNevigation->IsMove(vMoveDistance + vCalPellPos))
                {
                    m_pTransformCom->LerpTurn(m_pTransformCom->GetUpVector(), vMoveDistance + vCalPellPos, XMConvertToRadians(180.f), fDeletaTime);
                    m_pTransformCom->ADD_Position(vMoveDistance);
                }
            }
        }
        break;
        }
    }
}

HRESULT CGreenMommoth::ADD_Components()
{
#pragma region 그린모스 충돌체
    COBBCollision::OBB_COLLISION_DESC ObbDesc = {};
    ObbDesc.pOwner = this;
    ObbDesc.vExtents = _float3(2.0f, 2.7f, 3.2f);
    ObbDesc.vCneter = _float3(0.f, ObbDesc.vExtents.y * 0.5f, 0.f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &ObbDesc)))
        return E_FAIL;
    m_pCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { OverlapEvent(vDir, pHitActor); });
   
#pragma endregion

#pragma region 그린모스 타겟 감지 센서
    CAiSenceComponent::AI_SENCE_COMPONENT_DESC AiSenceDesc = {};
    AiSenceDesc.pOwner = this;
    AiSenceDesc.fAiSearchRadius = 360.f;
    AiSenceDesc.fAiTargetSearchDistance = 15.f;
    AiSenceDesc.fAiTargetLostDistance = 25.f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_Component_AiSence"), TEXT("AiSence_Com"), (CComponent**)&m_pAiSenceCom, &AiSenceDesc)))
        return E_FAIL;

    m_pAiSenceCom->Bind_TargetSearch([this](CGameObject* pSearchObject) { TargetSearchBegin(pSearchObject); });
    m_pAiSenceCom->Bind_TargetDetected([this](CGameObject* pSearchObject) { TargetDetected(pSearchObject); });
    m_pAiSenceCom->Bind_TargetLost([this](CGameObject* pSearchObject) { TargetLost(pSearchObject); });
#pragma endregion

    CChaseComponent::CHASE_INITIALIZE_DESC ChaseInitDesc = {};
    ChaseInitDesc.pOwnerTransform = m_pTransformCom;
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Chase"), TEXT("Chase_Com"), (CComponent**)&m_pChase, &ChaseInitDesc)))
        return E_FAIL;

    CCombatComponent::COMBAT_COMPONENT_DESC CombatDesc = {};
    CombatDesc.pOwner = this;
    CombatDesc.fChangeTargetDistance = 200.f;
    CombatDesc.fLostTargetTime = 5.0f;
    CombatDesc.CallBackFunction = [this](_float fDeletaTime, CGameObject* pTarget) { CombatAction(fDeletaTime, pTarget); };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Combat"), TEXT("Combat_Com"), (CComponent**)&m_pCombatCom, &CombatDesc)))
        return E_FAIL;

    SettingNavigation();
    return S_OK;
}

HRESULT CGreenMommoth::ADD_PartObjects()
{
    CPellBody::PARTOBJECT_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };

    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GrassMommoth_Body_Default"), TEXT("Part_Body"), &Desc)))
        return E_FAIL;

    m_pPellBody = static_cast<CPellBody*>(FindPartObject(TEXT("Part_Body")));
    return S_OK;
}

HRESULT CGreenMommoth::Setup_PellFsm()
{
    CPellStateMachine::FSM_DESC FSMDesc = {};
    FSMDesc.iLayerSize = 2;
    FSMDesc.pOwner = this;

    m_pPellFsm = CPellStateMachine::Create();
    if (FAILED(m_pPellFsm->Initialize(&FSMDesc)))
        return E_FAIL;
    return S_OK;
}

void CGreenMommoth::TargetSearchBegin(CGameObject* pSearchObject)
{
    m_pCombatCom->ADD_TargetObject(pSearchObject);
}

void CGreenMommoth::TargetLost(CGameObject* pSearchObject)
{
    m_pCombatCom->Remove_TargetObject(pSearchObject);

    //타겟을 잃으면 체력바를 보여주지 말자.
    auto pGamePlayHUD = static_cast<CGamePlayHUD*>(m_pGameInstance->GetCurrentHUD());
    pGamePlayHUD->HiddenBossHealthBar();
}

void CGreenMommoth::TargetDetected(CGameObject* pGameObject)
{
    auto pGamePlayHUD = static_cast<CGamePlayHUD*>(m_pGameInstance->GetCurrentHUD());
    pGamePlayHUD->SetBossHealthBar(this);
}

void CGreenMommoth::OverlapEvent(_float3 vDir, CGameObject* pHitObject)
{
  
}

CGreenMommoth* CGreenMommoth::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGreenMommoth* pGrassMommoth = new CGreenMommoth(pDevice, pContext);
    if (FAILED(pGrassMommoth->Initalize_Prototype()))
    {
        Safe_Release(pGrassMommoth);
        MSG_BOX("CREATE FAIL : Grass Mommoth");
    }
    return pGrassMommoth;
}

CGameObject* CGreenMommoth::Clone(void* pArg)
{
    CGreenMommoth* pGrassMommoth = new CGreenMommoth(*this);
    if (FAILED(pGrassMommoth->Initialize(pArg)))
    {
        Safe_Release(pGrassMommoth);
        MSG_BOX("CLONE FAIL : Grass Mommoth");
    }
    return pGrassMommoth;
}

void CGreenMommoth::Free()
{
    __super::Free();

    Safe_Release(m_pAiSenceCom);
}
