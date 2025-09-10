#include "PellBase.h"

#include "GameInstance.h"
#include "PellStateMachine.h"
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
}

void CPellBase::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CPellBase::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CPellBase::Render()
{
    return E_NOTIMPL;
}

void CPellBase::PellAction(_float fDeletaTime)
{
    m_fAccActionTime += fDeletaTime;

    if (m_fAccActionTime >= m_fActionTime)
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

        m_fAccActionTime = 0.f;
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
                // 이거는 여기에서 목표지점에 가까이가거나하면 모드 전환
                // 이거도 추적이라는 컴포넌트로 관리할거임
                m_pPellFsm->ChangeState(TEXT("BodyLayer"), TEXT("Idle"));
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
                //여기서 이제 이동 로직에의해서 동작





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

CGameObject* CPellBase::Clone(void* pArg)
{
    return nullptr;
}

void CPellBase::Free()
{
    __super::Free();
}
