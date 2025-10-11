#include "GreenMommoth.h"
#include "GameInstance.h"

#include "TerrainManager.h"

#pragma region Client Compoent
#include "CombatComponent.h"
#include "DefenceBossComponent.h"
#include "AiSenceComponent.h"
#pragma endregion

#include "PellStateMachine.h"
#include "PellAttackState.h"
#include "PellPatrolState.h"

#include "PellBody.h"

CGreenMommoth::CGreenMommoth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPellBase(pDevice, pContext)
{
}

CGreenMommoth::CGreenMommoth(const CGreenMommoth& rhs) :
    CPellBase(rhs)
{
    m_PellID = 4;
}

HRESULT CGreenMommoth::Initalize_Prototype()
{
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

    m_eTeam = PELL_TEAM::NEUTRAL;
    return S_OK;
}

void CGreenMommoth::Priority_Update(_float fDeletaTime)
{
    CContainerObject::Priority_Update(fDeletaTime);

    auto pTarget = m_pCombatCom->GetCurrentTarget();
    if(pTarget)
    {
        m_pCombatCom->UpdateTarget();
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
                m_pPellFsm->CombatStateReset();
                m_pPellFsm->ChangeState(TEXT("Body_Layer"), TEXT("Idle"));
            }
        }
    }

    m_pNevigation->ComputeHeight(m_pTransformCom, false);
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

void CGreenMommoth::Update(_float fDeletaTime)
{
    m_pPellBody->PellPlayAnimation(m_pPellFsm->GetAnimationName().c_str(), m_bIsLoop);
    CContainerObject::Update(fDeletaTime);
}

void CGreenMommoth::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        CContainerObject::Late_Update(fDeletaTime);
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
        AttackDesc.AttackData = &m_PellInfo.DefaultSkill;
        AttackDesc.fSkillMoveSpeed = &m_fPellMoveSpeed;
        AttackDesc.IsSpaceOut = false;
        m_bIsAction = true;

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
    ObbDesc.vExtents = _float3(0.5f, 2.7f, 3.2f);
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

    auto Object = m_pGameInstance->GetAllObejctToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Terrian"))->begin();
    auto OriginNav = static_cast<CNavigation*>((*Object)->Find_Component(TEXT("NaviMesh_Com")));
    CNavigation::NAVIGATION_DESC Desc = {};
    _float3 vPos = m_pTransformCom->GetPosition();
    Desc.iCurrentCellIndex = (int)m_pGameInstance->Random(100.f, 150.f);

    m_pTransformCom->SetPosition(OriginNav->CellCenterPos(Desc.iCurrentCellIndex));
    m_pNevigation = static_cast<CNavigation*>(OriginNav->Clone(&Desc));

    Safe_AddRef(m_pNevigation);
    m_pComponentMap.emplace(TEXT("NaviMesh_Com"), m_pNevigation);

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
    CPellStateMachine::PELLFSM_DESC FSMDesc = {};
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
