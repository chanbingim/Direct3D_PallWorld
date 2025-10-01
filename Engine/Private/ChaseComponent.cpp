#include "ChaseComponent.h"

#include "GameInstance.h"

CChaseComponent::CChaseComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

CChaseComponent::CChaseComponent(const CChaseComponent& rhs) : 
    CComponent(rhs)
{
}

HRESULT CChaseComponent::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CChaseComponent::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    CHASE_INITIALIZE_DESC* ChaseInitDesc = static_cast<CHASE_INITIALIZE_DESC*>(pArg);
    m_pOwnerTransform = ChaseInitDesc->pOwnerTransform;
    return S_OK;
}

void CChaseComponent::ComputeLerpPoint(_float fDeletaTime, _float3& vOutDir, _float3& vOutMovePoint)
{
    _float3 vOwnerPosition = m_pOwnerTransform->GetPosition();
    _float3 vTargetPosition = m_pTargetTransform->GetPosition();

    _vector vDir = XMLoadFloat3(&vTargetPosition) - XMLoadFloat3(&vOwnerPosition);
    vDir = XMVector3Normalize(vDir);

    XMStoreFloat3(&vOutDir, vDir);
    XMStoreFloat3(&vOutMovePoint, vDir * fDeletaTime * (*m_fChaseSpeed));
}

void CChaseComponent::SetChase(const CHASE_DESC& ChaseInfo)
{
    m_pTargetTransform = ChaseInfo.pTargetTransform;
    m_fChaseSpeed = ChaseInfo.fChaseSpeed;
}

_bool CChaseComponent::IsEndChase()
{
    return m_bIsChase;
}

CChaseComponent* CChaseComponent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CChaseComponent* pChase = new CChaseComponent(pDevice, pContext);
    if (FAILED(pChase->Initialize_Prototype()))
    {
        Safe_Release(pChase);
        MSG_BOX("CREATE FAIL : Chase");
    }
    return pChase;
}

CComponent* CChaseComponent::Clone(void* pArg)
{
    CChaseComponent* pChase = new CChaseComponent(*this);
    if (FAILED(pChase->Initialize(pArg)))
    {
        Safe_Release(pChase);
        MSG_BOX("CLONE FAIL : Chase");
    }
    return pChase;
}

void CChaseComponent::Free()
{
    __super::Free();
}
