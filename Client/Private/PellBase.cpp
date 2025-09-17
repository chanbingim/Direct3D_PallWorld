#include "PellBase.h"

#include "GameInstance.h"

#include "PellStateMachine.h"
#include "NeturalPellInfo.h"
#include "PellBody.h"
#include "Recovery.h"

CPellBase::CPellBase(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CContainerObject(pGraphic_Device, pDeviceContext)
{
   
}

CPellBase::CPellBase(const CPellBase& rhs) :
    CContainerObject(rhs)
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
}

HRESULT CPellBase::Render()
{
    return E_NOTIMPL;
}

HRESULT CPellBase::ADD_PellInfoUI()
{
    CNeturalPellInfo::NETURAL_PELL_DESC PellInfoDesc = {};
    PellInfoDesc.pOwner = this;
    PellInfoDesc.vScale = { 2.5f, 0.5f, 0.f };

    PellInfoDesc.vPosition = { 3.f, 3.f, 0.f };
    m_pNeturalPellUI = static_cast<CNeturalPellInfo*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PellInfo_UI"), &PellInfoDesc));

    return S_OK;
}

void CPellBase::PellPlayFSM(_float fDeletaTime)
{
    m_fAccActionTime += fDeletaTime;
    
    if (m_fAccActionTime >= m_fActionTime)
    {
        PellChiceAction();
        m_fAccActionTime = 0.f;
    }
    else
    {
        if(PELL_TEAM::NEUTRAL == m_eTeam)
            ShowPellInfo();
        PellTackingAction();
    }
       
        
    m_pPellFsm->Update(fDeletaTime);
}

void CPellBase::PellChiceAction()
{
    switch (m_eTeam)
    {
    case PELL_TEAM::FRENDLY:
        ActionFrendly();
        break;
    case PELL_TEAM::NEUTRAL:
        ActionNeutral();
        break;
    case PELL_TEAM::ENEMY:
        ActionEnemy();
        break;
    }
}

void CPellBase::PellTackingAction()
{
    const CPellStateMachine::PELL_STATE& State = m_pPellFsm->GetState();
    if (CPellStateMachine::COMBAT_ACTION::END == State.eCombat_State)
    {
        if (m_bIsAction)
        {
            switch (State.eMove_State)
            {
            case CPellStateMachine::MOVE_ACTION::PATROL:
            {
                // 이거는 여기에서 목표지점에 가까이가거나하면 모드 전환
                // 이거도 추적이라는 컴포넌트로 관리할거임
                _vector vTargetPos = XMLoadFloat3(&m_vTargetPoint);
                _float3 vCurPos = m_pTransformCom->GetPosition();
                _vector vPos = XMLoadFloat3(&vCurPos);
                if (1 > XMVectorGetX(XMVector3Length(vTargetPos - vPos)))
                {
                    if (m_PathFinding.empty())
                    {
                        m_vTargetPoint = { -1.f, -1.f, -1.f };
                        m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Idle"));
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

void CPellBase::ActionFrendly()
{

}

void CPellBase::ActionNeutral()
{
    const CPellStateMachine::PELL_STATE& State = m_pPellFsm->GetState();
    if (CPellStateMachine::COMBAT_ACTION::END == State.eCombat_State)
    {
        if (m_bIsAction)
        {
            switch (State.eMove_State)
            {
            case CPellStateMachine::MOVE_ACTION::PATROL :
            {
               
              
            }
            break;
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
                m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Rest"));
                m_pRecovery->SetRecovery(true);
                m_bIsAction = true;
            }
            else
            {
                _float3 vEndPoint = m_pTransformCom->GetPosition();
                vEndPoint.x += m_pGameInstance->Random(-5.f, 5.f);
                vEndPoint.z += m_pGameInstance->Random(-5.f, 5.f);

                 m_pNevigation->ComputePathfindingAStar(m_pTransformCom->GetPosition(), vEndPoint, &m_PathFinding);
                 if (!m_PathFinding.empty())
                 {
                     auto iter = m_PathFinding.begin();
                     m_vTargetPoint = *iter;
                     m_PathFinding.erase(iter);
                     m_bIsAction = true;
                     m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Patrol"));
                 }
            }
        }
    }
    else
    {
        //전투 중이라면 여기서 상태 진행





    }
   
}

void CPellBase::ActionEnemy()
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

CGameObject* CPellBase::Clone(void* pArg)
{
    return nullptr;
}

void CPellBase::Free()
{
    __super::Free();

    Safe_Release(m_pPellFsm);
    Safe_Release(m_pRecovery);
    Safe_Release(m_pNeturalPellUI);
    Safe_Release(m_pNevigation);
}
