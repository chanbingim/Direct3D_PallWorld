#include "ElectPanda.h"

#include "GameInstance.h"

#include "TerrainManager.h"
#include "PellStateMachine.h"

#pragma region Client Compoent
#include "Recovery.h"
#include "CombatComponent.h"
#pragma endregion

#include "PellSkillManager.h"
#include "PellAttackState.h"

#include "ElectricPandaBody.h"

CElectPanda::CElectPanda(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
    m_PellID = 2;
}

CElectPanda::CElectPanda(const CElectPanda& rhs) :
    CPellBase(rhs)
{
}

HRESULT CElectPanda::Initalize_Prototype()
{


    return S_OK;
} 

HRESULT CElectPanda::Initialize(void* pArg)
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

    m_eTeam = ACTOR_TEAM::NEUTRAL;
    m_PellInfo.PartnerSkillList.push_back(*CPellSkillManager::GetInstance()->FindPellData(6));
    m_PellInfo.PartnerSkillList.push_back(*CPellSkillManager::GetInstance()->FindPellData(7));
    return S_OK;
}

void CElectPanda::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
    PellPlayFSM(fDeletaTime);

    const CPellStateMachine::PELL_STATE& State = m_pPellFsm->GetState();
    _vector vPos{}, vDir{}, vCalMovePoint;
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
                        vCalMovePoint = vDir * m_fPellMoveSpeed * fDeletaTime;

                        _float3 vMovePoint = {};
                        XMStoreFloat3(&vMovePoint, vPos + vCalMovePoint);
                        if (false == m_pTerrainManager->UpdateChunk(m_szChunkName.c_str(), vMovePoint))
                            SettingNavigation();

                        if (m_pNevigation->IsMove(vPos + vCalMovePoint))
                        {
                            m_pTransformCom->LerpTurn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMLoadFloat3(&vCurPos) + vDir, XMConvertToRadians(180.f), fDeletaTime);
                            m_pTransformCom->ADD_Position(vCalMovePoint);
                        }
                    }
                }
            }
        }
    }
    else
    {
        _uInt i =  m_pPellFsm->GetStatePhase(TEXT("CombatLayer"));
        auto pTarget = m_pCombatCom->GetCurrentTarget();
        if (pTarget)
        {
            _float3 vTargetPos = pTarget->GetTransform()->GetPosition();
            _float3 vCurPos = m_pTransformCom->GetPosition();

            _vector vCalTargetPos = XMLoadFloat3(&vTargetPos);
            vPos = XMLoadFloat3(&vCurPos);
            vDir = XMVector3Normalize(vCalTargetPos - vPos);
            if (i < 3)
                m_pTransformCom->LerpTurn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMLoadFloat3(&vCurPos) + vDir, XMConvertToRadians(180.f), fDeletaTime);
        }

    }


    if (CPellStateMachine::MOVE_ACTION::CARRY > State.eMove_State)
        m_pNevigation->ComputeHeight(m_pTransformCom, false);

    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));

}

void CElectPanda::Update(_float fDeletaTime)
{
    if (PELL_STORAGE_STATE::PARTNER_PELL < m_PellInfo.ePellStorageState)
        return;

    _bool      bIsAnimLoop = {};
    if (CPellStateMachine::COMBAT_ACTION::END != m_pPellFsm->GetState().eCombat_State)
        bIsAnimLoop = m_pPellFsm->GetLayerAnimLoop(TEXT("CombatLayer"));
    else
        bIsAnimLoop = m_pPellFsm->GetLayerAnimLoop(TEXT("Body_Layer"));

    m_pPellBody->PellPlayAnimation(m_pPellFsm->GetAnimationName().c_str(), bIsAnimLoop);
    __super::Update(fDeletaTime);

}

void CElectPanda::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        if (PELL_STORAGE_STATE::PARTNER_PELL < m_PellInfo.ePellStorageState)
            return;

        __super::Late_Update(fDeletaTime);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CElectPanda::Render()
{
    __super::Render();

    return S_OK;
}

void CElectPanda::Damage(void* pArg, CActor* pDamagedActor)
{
    __super::Damage(pArg, pDamagedActor);
}

void CElectPanda::CombatAction(_float fDeletaTime, CGameObject* pTarget)
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

    // 일렉 펜더는 확률로 일정 스킬을 사용하게 변경
    _uInt iSkillIndex =  m_PellInfo.PartnerSkillList.size();
    if (0 < iSkillIndex)
    {
        _uInt iRandomIndex = static_cast<_Int>(m_pGameInstance->Random(0, 100.f)) % iSkillIndex;
        AttackDesc.AttackData = &m_PellInfo.PartnerSkillList[1];
        static_cast<CElectricPandaBody*>(m_pPellBody)->SetSelectSkillIndex(AttackDesc.AttackData->iSkillID);
    }
   
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

HRESULT CElectPanda::ADD_Components()
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

    SettingNavigation();
    return S_OK;
}

HRESULT CElectPanda::ADD_PartObjects()
{
    CPellBody::PARTOBJECT_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };

    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricPanda_Body_Default"), TEXT("Part_Body"), &Desc)))
        return E_FAIL;

    m_pPellBody = static_cast<CPellBody*>(FindPartObject(TEXT("Part_Body")));
    return S_OK;
}

HRESULT CElectPanda::Setup_PellFsm()
{
    CPellStateMachine::FSM_DESC FSMDesc = {};
    FSMDesc.iLayerSize = 2;
    FSMDesc.pOwner = this;

    m_pPellFsm = CPellStateMachine::Create();
    if (FAILED(m_pPellFsm->Initialize(&FSMDesc)))
        return E_FAIL;
    return S_OK;
}

void CElectPanda::OverlapEvent(_float3 vDir, CGameObject* pHitObject)
{
    auto State = m_pPellFsm->GetState();

    if (State.bIsAttacking)
    {
      
    }
    else
    {
        __super::OverlapEvent(vDir, pHitObject);
    }
}

CElectPanda* CElectPanda::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CElectPanda* pElectPanda = new CElectPanda(pGraphic_Device, pDeviceContext);
    if (FAILED(pElectPanda->Initalize_Prototype()))
    {
        Safe_Release(pElectPanda);
        MSG_BOX("CREATE FAIL : ElECT PANDA");
    }
    return pElectPanda;
}

CGameObject* CElectPanda::Clone(void* pArg)
{
    CElectPanda* pElectPanda = new CElectPanda(*this);
    if (FAILED(pElectPanda->Initialize(pArg)))
    {
        Safe_Release(pElectPanda);
        MSG_BOX("CLONE FAIL : ElECT PANDA");
    }
    return pElectPanda;
}

void CElectPanda::Free()
{
    __super::Free();
}
