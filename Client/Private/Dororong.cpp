#include "Dororong.h"

#include "GameInstance.h"

#include "TerrainManager.h"

#include "PellStateMachine.h"
#include "PellStateStun.h"

#pragma region Client Compoent
#include "Recovery.h"
#include "CombatComponent.h"
#pragma endregion

#include "PellAttackState.h"
#include "PellBody.h"

CDororong::CDororong(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
}

CDororong::CDororong(const CDororong& rhs) :
    CPellBase(rhs)
{
    m_PellID = 0;
}

HRESULT CDororong::Initalize_Prototype()
{


    return S_OK;
}

HRESULT CDororong::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_PellFsm()))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_PartObjects()))
        return E_FAIL;

    if (FAILED(ADD_PellInfoUI()))
        return E_FAIL;

    m_eTeam = PELL_TEAM::NEUTRAL;
    return S_OK;
}

void CDororong::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
    PellPlayFSM(fDeletaTime);

   /* if (m_pRecovery->GetRecovering())
        m_PellInfo.CurStemina += m_pRecovery->Update(fDeletaTime);
    else
        m_PellInfo.CurStemina -= 0.1f;*/

    const CPellStateMachine::PELL_STATE& State = m_pPellFsm->GetState();
    _vector vPos{}, vDir{}, vMovePoint;
    if (CPellStateMachine::COMBAT_ACTION::END == State.eCombat_State)
    {
        if (PELL_STORAGE_STATE::PARTNER_PELL != m_PellInfo.ePellStorageState)
        {
            if (CPellStateMachine::MOVE_ACTION::PATROL == State.eMove_State)
            {
                _vector vTarget = XMLoadFloat3(&m_vTargetPoint);
                if (!XMVector3Equal(vTarget, XMVectorZero()))
                {
                    if (0 != m_fPellMoveSpeed)
                    {
                        _float3 vCurPos = m_pTransformCom->GetPosition();
                        vPos = XMLoadFloat3(&vCurPos);
                        vPos.m128_f32[1] = 0.f;
                        vDir = XMVector3Normalize(vTarget - vPos);

                        vMovePoint = vDir * m_fPellMoveSpeed * fDeletaTime;
                        if (m_pNevigation->IsMove(vPos + vMovePoint))
                        {
                            m_pTransformCom->LerpTurn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMLoadFloat3(&vCurPos) + vDir, XMConvertToRadians(180.f), fDeletaTime);
                            m_pTransformCom->ADD_Position(vMovePoint);
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (CPellStateMachine::COMBAT_ACTION::ATTACK == State.eCombat_State)
        {
            _float3 vCurPos = m_pTransformCom->GetPosition();
            vPos = XMLoadFloat3(&vCurPos);
            if (0 != m_fPellMoveSpeed)
            {
                _float3 ChaseDir, ChaseMovePoint;
                m_pChase->ComputeLerpPoint(fDeletaTime, ChaseDir, ChaseMovePoint);
                if (m_pNevigation->IsMove(vPos + XMLoadFloat3(&ChaseMovePoint)))
                {
                    m_pTransformCom->LerpTurn(XMVectorSet(0.f, 1.f, 0.f, 0.f), vPos + XMLoadFloat3(&ChaseMovePoint), XMConvertToRadians(180.f), fDeletaTime);
                    m_pTransformCom->ADD_Position(XMLoadFloat3(&ChaseMovePoint));
                }
            }
        }
    }

    if (CPellStateMachine::MOVE_ACTION::CARRY > State.eMove_State)
        m_pNevigation->ComputeHeight(m_pTransformCom, false);

    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

void CDororong::Update(_float fDeletaTime)
{
    if (PELL_STORAGE_STATE::PARTNER_PELL < m_PellInfo.ePellStorageState)
        return;

    m_pPellBody->PellPlayAnimation(m_pPellFsm->GetAnimationName().c_str(), m_bIsLoop);
    __super::Update(fDeletaTime);
 
}

void CDororong::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        if (PELL_STORAGE_STATE::PARTNER_PELL < m_PellInfo.ePellStorageState)
            return;

        __super::Late_Update(fDeletaTime);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CDororong::Render()
{
    __super::Render();
    return S_OK;
}

void CDororong::Damage(void* pArg, CActor* pDamagedActor)
{
    __super::Damage(pArg, pDamagedActor);

}

void CDororong::CombatAction(_float fDeletaTime, CGameObject* pTarget)
{
    // 일반 공격 및 스킬 공격
    // 둘다 안되면 그냥 이동
    // 타겟과의 거리를 통해서 스킬 사거리에 들어와있는지를 확인
    // 스킬 데이터를 어딘가에서 들고있고 prototpye 했을때
    // 어차피 스킬데이터를 다 찾아서 들고있다면 될거같음
    m_PathFinding.clear();
    _float3 vTargetPos = pTarget->GetTransform()->GetPosition();
    _float3 vPos = m_pTransformCom->GetPosition();

    _float fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vTargetPos) - XMLoadFloat3(&vPos)));
    if (m_PellInfo.fPellAttackRange > fDistance)
    {
        //점프나 이동으로 거리를 벌린뒤에 행동 추가로 함
    }
   CPellAttackState::PELL_ATTACK_STATE_DESC AttackDesc = {};
    AttackDesc.ActPell = this;
    AttackDesc.AttackData = &m_PellInfo.DefaultSkill;
    AttackDesc.fSkillMoveSpeed = &m_fPellMoveSpeed;
    AttackDesc.IsSpaceOut = false;
    m_bIsAction = true;

    /*  m_pNevigation->ComputePathfindingAStar(m_pTransformCom->GetPosition(), vTargetPos, &m_PathFinding);
    m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Patrol"));*/

    CChaseComponent::CHASE_DESC ChaseDesc = {};
    ChaseDesc.pTargetTransform = pTarget->GetTransform();
    ChaseDesc.fChaseSpeed = &m_fPellMoveSpeed;
    m_pChase->SetChase(ChaseDesc);
    
    m_PathFinding.clear();
    m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Idle"));

    m_pPellFsm->ChangeState(TEXT("CombatLayer"), TEXT("Attack"), &AttackDesc);
    m_pPellFsm->SetAttack(true);
}

HRESULT CDororong::ADD_Components()
{
    CRecovery::RECOVERY_DESC RecoverDesc;
    RecoverDesc.pOwner = this;
    RecoverDesc.fRecoveryTime = 1.0f;
    RecoverDesc.fPerTickRecovery = 10.f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("GamePlay_Component_Recovery"), TEXT("Recovery_Com"), (CComponent**)&m_pRecovery, &RecoverDesc)))
        return E_FAIL;

    CSphereCollision::SPEHRE_COLLISION_DESC SphereDesc = {};
    ZeroMemory(&SphereDesc, sizeof(CSphereCollision::SPEHRE_COLLISION_DESC));
    SphereDesc.pOwner = this;
    SphereDesc.Radius = 0.5f;
    SphereDesc.vCneter = _float3(0.f, SphereDesc.Radius, 0.f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionSphere"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &SphereDesc)))
        return E_FAIL;
    m_pCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { OverlapEvent(vDir, pHitActor); });

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
    Desc.iCurrentCellIndex = OriginNav->Find_Cell(XMLoadFloat3(&vPos));

    m_pTransformCom->SetPosition(OriginNav->CellCenterPos(Desc.iCurrentCellIndex));
    m_pNevigation = static_cast<CNavigation*>(OriginNav->Clone(&Desc));

    Safe_AddRef(m_pNevigation);
    m_pComponentMap.emplace(TEXT("NaviMesh_Com"), m_pNevigation);

    return S_OK;
}

HRESULT CDororong::ADD_PartObjects()
{
    CPellBody::PARTOBJECT_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };

    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Drorong_Body_Default"), TEXT("Part_Body"), &Desc)))
        return E_FAIL;

    m_pPellBody = static_cast<CPellBody *>(FindPartObject(TEXT("Part_Body")));
    return S_OK;
}

HRESULT CDororong::Setup_PellFsm()
{
    CPellStateMachine::FSM_DESC FSMDesc = {};
    FSMDesc.iLayerSize = 2;
    FSMDesc.pOwner = this;

    m_pPellFsm = CPellStateMachine::Create();
    if (FAILED(m_pPellFsm->Initialize(&FSMDesc)))
        return E_FAIL;
    return S_OK;
}

void CDororong::OverlapEvent(_float3 vDir, CGameObject* pHitObject)
{
    auto State = m_pPellFsm->GetState();

    if (State.bIsAttacking)
    {
        DEFAULT_DAMAGE_DESC DamageDesc = {};
        DamageDesc.fDmaged = (_float)m_PellInfo.DefaultSkill.iSkillDamage;

       auto Hit =  static_cast<CActor*>(pHitObject);
       if (Hit)
       {
           Hit->Damage(&DamageDesc, this);

           CPellStateStun::PELL_STATE_STUN_STATE StunStateDesc = {};
           StunStateDesc.StunTime = 0.7f;
           StunStateDesc.ActionFunction = [&](_float fDeletaTime, _bool bIsEndStun) { StunAction(fDeletaTime, bIsEndStun); };
           
           _float3  vPellPosition = m_pTransformCom->GetPosition();
           _float3  vHitActorPoint = pHitObject->GetTransform()->GetPosition();
           
           _vector vCalculationPosition = XMLoadFloat3(&vPellPosition);
           _vector vCalculationHitActorPosition = XMLoadFloat3(&vHitActorPoint);

           _vector vDir = vCalculationHitActorPosition - vCalculationPosition;
           vDir.m128_f32[1] = 0;
           vDir = XMVector3Normalize(vDir);

           _float RandomSeta = m_pGameInstance->Random(-XM_PIDIV4, XM_PIDIV4);
            vDir.m128_f32[0] *= cos(RandomSeta);
            vDir.m128_f32[2] *= sin(RandomSeta);

            XMStoreFloat3(&m_HitReflectionDir, vDir);
           m_pPellFsm->ChangeState(TEXT("CombatLayer"), TEXT("Stun"), &StunStateDesc);
           m_pPellFsm->SetAttack(false);
           m_bIsAction = false;
       }
    }
    else
    {
        __super::OverlapEvent(vDir, pHitObject);
    }
}

void CDororong::StunAction(_float fDeletaTime, _bool bIsStunEnd)
{
    if (bIsStunEnd)
    {
        m_pPellFsm->CombatStateReset();
        m_pPellFsm->SetAttack(false);
        m_pTransformCom->SetRotation(_float3( 0.f, 0.f, 0.f ));
        m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Idle"));
    }
    else
    {
        // 여기서 이제 동글동글 굴러가는 효과를 낼거임
        // 부딪힌 점으로 부터 
        m_pTransformCom->ADD_Position(XMLoadFloat3(&m_HitReflectionDir) * fDeletaTime * 3.f);
        m_pTransformCom->Turn(m_pTransformCom->GetRightVector(), 3.f, fDeletaTime);
    }
}

CDororong* CDororong::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CDororong* pDororong = new CDororong(pGraphic_Device, pDeviceContext);
    if (FAILED(pDororong->Initalize_Prototype()))
    {
        Safe_Release(pDororong);
        MSG_BOX("CREATE FAIL : DRORONG");
    }
    return pDororong;
}

CGameObject* CDororong::Clone(void* pArg)
{
    CDororong* pDororong = new CDororong(*this);
    if (FAILED(pDororong->Initialize(pArg)))
    {
        Safe_Release(pDororong);
        MSG_BOX("CLONE FAIL : DRORONG");
    }
    return pDororong;
}

void CDororong::Free()
{
    __super::Free();
}
