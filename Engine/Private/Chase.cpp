#include "Chase.h"

#include "GameInstance.h"

CChase::CChase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

CChase::CChase(const CChase& rhs) : 
    CComponent(rhs)
{
}

HRESULT CChase::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CChase::Initialize(void* pArg)
{
    return S_OK;
}

_float3 CChase::ComputeLerpPoint(_float fDeletaTime)
{
    _float3 vResult = {};
    _vector vStartPoint = XMLoadFloat3(&m_vStartPoint);
    _vector vTargetPoint = XMLoadFloat3(&m_vTargetPoint);

    m_fAccTime += m_fTickPerAcc;
    _vector vLerpPoint = XMVectorLerp(vStartPoint, vTargetPoint, m_fAccTime);
    XMStoreFloat3(&vResult, vLerpPoint);

    return vResult;
}

void CChase::SetChase(const CHASE_DESC& ChaseInfo)
{
    m_vStartPoint = ChaseInfo.vStartPoint;
    m_vTargetPoint = ChaseInfo.vTargetPoint;
    m_fChaseTime = ChaseInfo.fChaseTime;

    m_fTickPerAcc = 1.f / m_fChaseTime;
    m_fAccTime = 0.f;
}

_bool CChase::IsEndChase()
{
    return m_bIsChase;
}

CChase* CChase::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CChase* pChase = new CChase(pDevice, pContext);
    if (FAILED(pChase->Initialize_Prototype()))
    {
        Safe_Release(pChase);
        MSG_BOX("CREATE FAIL : Chase");
    }
    return pChase;
}

CComponent* CChase::Clone(void* pArg)
{
    CChase* pChase = new CChase(*this);
    if (FAILED(pChase->Initialize(pArg)))
    {
        Safe_Release(pChase);
        MSG_BOX("CLONE FAIL : Chase");
    }
    return pChase;
}

void CChase::Free()
{
    __super::Free();
}
