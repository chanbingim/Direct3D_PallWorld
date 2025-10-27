#include "AiSenceComponent.h"

#include "GameInstance.h"
#include "Level.h"

CAiSenceComponent::CAiSenceComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

CAiSenceComponent::CAiSenceComponent(const CAiSenceComponent& Prototype) :
    CComponent(Prototype)
{
}

HRESULT CAiSenceComponent::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAiSenceComponent::Initialize(void* pArg)
{
    AI_SENCE_COMPONENT_DESC* pSenceComDesc = static_cast<AI_SENCE_COMPONENT_DESC*>(pArg);
    m_pOwner = pSenceComDesc->pOwner;
    m_fAiSearchRadius = pSenceComDesc->fAiSearchRadius;
    m_fAiTargetSearchDistance = pSenceComDesc->fAiTargetSearchDistance;
    m_fAiTargetLostDistance = pSenceComDesc->fAiTargetLostDistance;

    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

 
    return S_OK;
}

void CAiSenceComponent::UpdatSenceComponent(_float fDeletaTime)
{
    _float4x4   vWorldMat = m_pOwner->GetTransform()->GetWorldMat();
    m_pTargetSearchCol->UpdateColiision(XMLoadFloat4x4(&vWorldMat));
    m_pGameInstance->ADD_CollisionList(m_pTargetSearchCol);
}

void CAiSenceComponent::Bind_TargetSearch(function<void(CGameObject*)> SearchFunc)
{
    m_SearchFunc = SearchFunc;
}

void CAiSenceComponent::Bind_TargetLost(function<void(CGameObject*)> TargetLostFunc)
{
    m_TargetLostFunc = TargetLostFunc;
}

void CAiSenceComponent::Bind_TargetDetected(function<void(CGameObject*)> Func)
{
    m_TargetDetectedFunc = Func;
}

void CAiSenceComponent::TargetSearch(_float3 vDir, CGameObject* pHitObject)
{
    if(nullptr != m_TargetDetectedFunc)
        m_TargetDetectedFunc(pHitObject);

    if (nullptr == dynamic_cast<CContainerObject*>(pHitObject))
        return;

    auto iter = m_pSearchList.find(pHitObject);
    if (iter != m_pSearchList.end())
        return;

    _float3 vOwnerPos = m_pOwner->GetTransform()->GetPosition();
    _float3 vHitObejctPos = pHitObject->GetTransform()->GetPosition();

    _vector vTargetDir = XMVector3Normalize(XMLoadFloat3(&vHitObejctPos) - XMLoadFloat3(&vOwnerPos));
    _vector vOwnerLook = m_pOwner->GetTransform()->GetLookVector();

    _float fScalar = XMVectorGetX(XMVector3Dot(vOwnerLook, vTargetDir));
    _float frad = acos(fScalar);

    if (0 < XMVectorGetY(XMVector3Cross(vOwnerLook, vTargetDir)))
    {
        if (frad < XMConvertToRadians(m_fAiSearchRadius))
        {
            m_pSearchList.insert(pHitObject);
            if (m_SearchFunc)
                m_SearchFunc(pHitObject);
        }
    }
}

void CAiSenceComponent::TargetLost(_float3 vDir, CGameObject* pHitObject)
{
    if (m_TargetLostFunc)
        m_TargetLostFunc(pHitObject);
}

HRESULT CAiSenceComponent::ADD_Components()
{
    CSphereCollision::SPEHRE_COLLISION_DESC SphereDesc = { };
    SphereDesc.pOwner = m_pOwner;
    SphereDesc.Radius = m_fAiTargetLostDistance;
    SphereDesc.vCneter = {0.f, SphereDesc.Radius * 0.5f, 0.f};

    m_pTargetSearchCol = static_cast<CCollision *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::COMPONENT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionSphere"), &SphereDesc));

    m_pTargetSearchCol->BindOverlappingEvent([this](_float3 vDir, CGameObject* pHitActor) { TargetSearch(vDir, pHitActor); });
    m_pTargetSearchCol->BindEndOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { TargetLost(vDir, pHitActor); });
    return S_OK;
}

CAiSenceComponent* CAiSenceComponent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAiSenceComponent* pSenceCom = new CAiSenceComponent(pDevice, pContext);
    if (FAILED(pSenceCom->Initialize_Prototype()))
    {
        Safe_Release(pSenceCom);
        MSG_BOX("CREATE FAIL : AI SENCE COM");
    }
    return pSenceCom;
}

CComponent* CAiSenceComponent::Clone(void* pArg)
{
    CAiSenceComponent* pSenceCom = new CAiSenceComponent(*this);
    if (FAILED(pSenceCom->Initialize(pArg)))
    {
        Safe_Release(pSenceCom);
        MSG_BOX("CLONE FAIL : AI SENCE COM");
    }
    return pSenceCom;
}

void CAiSenceComponent::Free()
{
    __super::Free();

    Safe_Release(m_pTargetSearchCol);
}
