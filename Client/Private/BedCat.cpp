#include "BedCat.h"

#include "GameInstance.h"

#include "TerrainManager.h"
#include "PellStateMachine.h"

#pragma region Client Compoent
#include "Recovery.h"
#include "CombatComponent.h"
#pragma endregion

#include "PellAttackState.h"
#include "PellBody.h"

CBedCat::CBedCat(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
   
}

CBedCat::CBedCat(const CBedCat& rhs) :
    CPellBase(rhs)
{
    m_PellID = 1;
}

HRESULT CBedCat::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBedCat::Initialize(void* pArg)
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

void CBedCat::Priority_Update(_float fDeletaTime)
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
        if (CPellStateMachine::MOVE_ACTION::PATROL == State.eMove_State)
        {
            _vector vTarget = XMLoadFloat3(&m_vTargetPoint);
            if (!XMVector3Equal(vTarget, XMVectorZero()))
            {
                _float3 vCurPos = m_pTransformCom->GetPosition();
                vCurPos.y = 0.f;
                vPos = XMLoadFloat3(&vCurPos);
                vDir = XMVector3Normalize(vTarget - vPos);

                vMovePoint = vDir * m_fPellMoveSpeed * fDeletaTime;
                if (m_pNevigation->IsMove(vPos + vMovePoint))
                {
                    m_pTransformCom->LerpTurn(XMVectorSet(0.f, 1.f, 0.f, 0.f), vTarget, XMConvertToRadians(180.f), fDeletaTime);
                    m_pTransformCom->ADD_Position(vMovePoint);
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

            _float3 ChaseDir, ChaseMovePoint;
            m_pChase->ComputeLerpPoint(fDeletaTime, ChaseDir, ChaseMovePoint);
            if (m_pNevigation->IsMove(vPos + XMLoadFloat3(&ChaseMovePoint)))
                m_pTransformCom->ADD_Position(XMLoadFloat3(&ChaseMovePoint));
        }
    }
    m_pNevigation->ComputeHeight(m_pTransformCom);
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

void CBedCat::Update(_float fDeletaTime)
{
    if (PELL_STORAGE_STATE::PARTNER_PELL < m_PellInfo.ePellStorageState)
        return;

    m_pPellBody->PellPlayAnimation(m_pPellFsm->GetAnimationName().c_str(), m_bIsLoop);
    __super::Update(fDeletaTime);
}

void CBedCat::Late_Update(_float fDeletaTime)
{
    if (PELL_STORAGE_STATE::PARTNER_PELL < m_PellInfo.ePellStorageState)
        return;

    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBedCat::Render()
{
    __super::Render();
    return S_OK;
}

void CBedCat::Damage(void* pArg, CActor* pDamagedActor)
{
    __super::Damage(pArg, pDamagedActor);
}

void CBedCat::CombatAction(CGameObject* pTarget)
{
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

HRESULT CBedCat::ADD_Components()
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
    CombatDesc.CallBackFunction = [this](CGameObject* pTarget) { CombatAction(pTarget); };
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

HRESULT CBedCat::ADD_PartObjects()
{
    CPellBody::PARTOBJECT_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };

    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BedCat_Body_Default"), TEXT("Part_Body"), &Desc)))
        return E_FAIL;

    m_pPellBody = static_cast<CPellBody*>(FindPartObject(TEXT("Part_Body")));

    return S_OK;
}

HRESULT CBedCat::Setup_PellFsm()
{
    CPellStateMachine::PELLFSM_DESC FSMDesc = {};
    FSMDesc.iLayerSize = 2;
    FSMDesc.pOwner = this;

    m_pPellFsm = CPellStateMachine::Create();
    if (FAILED(m_pPellFsm->Initialize(&FSMDesc)))
        return E_FAIL;
    return S_OK;
}

void CBedCat::OverlapEvent(_float3 vDir, CGameObject* pHitObject)
{
    auto State = m_pPellFsm->GetState();

    if (State.bIsAttacking)
    {
        DEFAULT_DAMAGE_DESC DamageDesc = {};
        DamageDesc.fDmaged = (_float)m_PellInfo.DefaultSkill.iSkillDamage;

        auto Hit = static_cast<CActor*>(pHitObject);
        if (Hit)
            Hit->Damage(&DamageDesc, this);
    }
}

CBedCat* CBedCat::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CBedCat* pBedCat = new CBedCat(pGraphic_Device, pDeviceContext);
    if (FAILED(pBedCat->Initalize_Prototype()))
    {
        Safe_Release(pBedCat);
        MSG_BOX("CREATE FAIL : BED CAT");
    }
    return pBedCat;
}

CGameObject* CBedCat::Clone(void* pArg)
{
    CBedCat* pBedCat = new CBedCat(*this);
    if (FAILED(pBedCat->Initialize(pArg)))
    {
        Safe_Release(pBedCat);
        MSG_BOX("CLONE FAIL : BED CAT");
    }
    return pBedCat;
}

void CBedCat::Free()
{
    __super::Free();
}
