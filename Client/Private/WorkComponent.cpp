#include "WorkComponent.h"

#include "GameInstance.h"

#include "PellBase.h"
#include "Architecture.h"
#include "MiningObject.h"

CWorkComponent::CWorkComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

CWorkComponent::CWorkComponent(const CWorkComponent& Prototype) :
    CComponent(Prototype)
{
}

HRESULT CWorkComponent::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CWorkComponent::Initialize(void* pArg)
{
    WORK_COMPONENT_DESC* pWorkComDesc = static_cast<WORK_COMPONENT_DESC*>(pArg);
    m_pOwner = pWorkComDesc->pOwner;

    return S_OK;
}

HRESULT CWorkComponent::Render()
{
#ifdef _DEBUG
    //나중에 경로 여기서 전부  보여주기



#endif // _DEBUG

    return S_OK;
}

void CWorkComponent::Update(_float fDeletaTime)
{
    if (m_pWorkTarget)
    {
        if (false == m_bIsFinished)
        {
            if (m_bIsGoToWork)
                GotToWork(fDeletaTime);
            else
                Working(fDeletaTime);
        }

        if (m_pWorkTarget->IsDead() || m_bIsFinished)
        {
            m_pWorkTarget = nullptr;
            m_bIsWorking = false;
            m_bIsGoToWork = false;
        }
    }
}

void CWorkComponent::SetWorkTargetObject(CWorkAbleObject* pWorkTarget, CNavigation* pNavigation)
{
    m_pWorkTarget = pWorkTarget;
    m_pNavigation = pNavigation;
    m_bIsFinished = false;

    WorkPathFinding();
}

void CWorkComponent::NextWorkPhase()
{
    m_bIsGoToWork = false;
    m_bIsWorking = true;
    m_SettingData = false;
}

const	CWorkAbleObject* CWorkComponent::GetWorkTarget()
{
    return m_pWorkTarget;
}

void CWorkComponent::WorkPathFinding()
{
    if (m_bIsWorking)
        return;

    if (m_pWorkTarget && m_pOwner)
    {
        m_pPathList.clear();

        _float3 vStartPos = m_pOwner->GetTransform()->GetPosition();
        _float3 vTargetPos = m_pWorkTarget->GetTransform()->GetPosition();

        m_pNavigation->ComputePathfindingAStar(vStartPos, vTargetPos, &m_pPathList);

        if (!m_pPathList.empty())
        {
            auto iter = m_pPathList.begin();
            m_vTargetPoint = *iter;
            m_bIsGoToWork = true;
        }
    }
}

void CWorkComponent::GotToWork(_float fDeletaTime)
{
    auto PellInfo = m_pOwner->GetPellInfo();
    _float3 vCurPos = m_pOwner->GetTransform()->GetPosition();

    _vector vPos = XMLoadFloat3(&vCurPos);
    vPos.m128_f32[1] = 0.f;
    _vector vTarget = XMLoadFloat3(&m_vTargetPoint);

    if (1 > XMVectorGetX(XMVector3Length(vTarget - vPos)))
    {
        if (!m_pPathList.empty())
        {
            m_vTargetPoint = m_pPathList.front();
            m_pPathList.pop_front();
            m_bIsGoToWork = true;
        }
        else
        {
            m_bIsGoToWork = false;
            m_bIsWorking = true;
            m_SettingData = false;
        }
    }
    else
    {
        _vector vDir = XMVector3Normalize(vTarget - vPos);
        _vector vMovePoint = vDir * PellInfo.fPellRunSpeed * fDeletaTime;
        if (m_pNavigation->IsMove(vPos + vMovePoint))
        {
            m_pOwner->GetTransform()->LerpTurn(m_pOwner->GetTransform()->GetUpVector(), vPos + vDir, XMConvertToRadians(180.f), fDeletaTime);
            m_pOwner->GetTransform()->ADD_Position(vMovePoint);
            m_pNavigation->ComputeHeight(m_pOwner->GetTransform());
        }
    }
}

void CWorkComponent::Working(_float fDeletaTime)
{
    PELL_WORK_TYPE TargetObjectWorkType = m_pWorkTarget->GetWorkAbleType();
    switch (TargetObjectWorkType)
    {
    case PELL_WORK_TYPE::COMMONE_WORK :
        CommoneWorkFunc(fDeletaTime);
        break;

    case PELL_WORK_TYPE::MINING:
        MinningWorkFunc(fDeletaTime);
        break;

    case PELL_WORK_TYPE::TRANSPORT:
        TransportWorkFunc(fDeletaTime);
        break;
    }

}

#pragma region WORK FUNC
void CWorkComponent::CommoneWorkFunc(_float fDeletaTime)
{
    //제작 작업을 수행하는 함수
    auto PellInfo = m_pOwner->GetPellInfo();
    auto pArchitecture = static_cast<CArchitecture*>(m_pWorkTarget);
   
    if (false == m_SettingData)
    {
        pArchitecture->PlusWorkSpeed(PellInfo.WorkSpeed);
        m_SettingData = true;
    }
    else
    {
        if (pArchitecture->IsBuildFinished())
        {
            pArchitecture->SubWorkSpeed(PellInfo.WorkSpeed);
            m_bIsFinished = true;
            m_bIsWorking = false;
        }
    }
}

void CWorkComponent::MinningWorkFunc(_float fDeletaTime)
{
    // 채광 작업을 수행하는 함수
    // 이건 돌 캐는거
    auto PellInfo = m_pOwner->GetPellInfo();
    auto pMiningObject = static_cast<CMiningObject*>(m_pWorkTarget);

    if (false == m_SettingData)
    {
        pMiningObject->PlusWorkSpeed(PellInfo.WorkSpeed);
        m_SettingData = true;
    }
    else
    {
        if (pMiningObject->IsMiningFinished())
        {
            pMiningObject->SubWorkSpeed(PellInfo.WorkSpeed);
            m_bIsFinished = true;
            m_bIsWorking = false;
        }
    }
}

void CWorkComponent::TransportWorkFunc(_float fDeletaTime)
{
    // 운반 작업을 수행하는 함수
    // 이거는 창고 오브젝트를 만들고 나서 구현
    auto PellInfo = m_pOwner->GetPellInfo();
  
}
#pragma endregion

CWorkComponent* CWorkComponent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWorkComponent* pWorkComponent = new CWorkComponent(pDevice, pContext);
    if (FAILED(pWorkComponent->Initialize_Prototype()))
    {
        Safe_Release(pWorkComponent);
        MSG_BOX("CREATE FAIL : WORK COMPONENT");
    }
    return pWorkComponent;
}

CWorkComponent* CWorkComponent::Clone(void* pArg)
{
    CWorkComponent* pWorkComponent = new CWorkComponent(*this);
    if (FAILED(pWorkComponent->Initialize(pArg)))
    {
        Safe_Release(pWorkComponent);
        MSG_BOX("CLONE FAIL : WORK COMPONENT");
    }
    return pWorkComponent;
}

void CWorkComponent::Free()
{
    __super::Free();
}
