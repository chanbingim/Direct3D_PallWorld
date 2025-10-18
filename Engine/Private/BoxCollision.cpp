#include "BoxCollision.h"
#include "SphereCollision.h"
#include "OBBCollision.h"

#include "Transform.h"

#include "DebugDraw.h"
#include "GameObject.h"

CBoxCollision::CBoxCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CCollision(pDevice, pContext)
{
}

CBoxCollision::CBoxCollision(const CBoxCollision& rhs) :
    CCollision(rhs)
{
}

HRESULT CBoxCollision::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    m_CollisionType = COLLISION_TYPE::BOX;
    return S_OK;
}

HRESULT CBoxCollision::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    BOX_COLLISION_DESC* Desc = static_cast<BOX_COLLISION_DESC*>(pArg);
    m_OriginBounding = new BoundingBox(Desc->vCneter, Desc->Extents);
    m_Bounding = new BoundingBox(*m_OriginBounding);

    return S_OK;
}

void CBoxCollision::UpdateColiision(_matrix WorldMatrix)
{
    m_bIsHit = false;
    WorldMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[0]);
    WorldMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[1]);
    WorldMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(WorldMatrix.r[2]);

    m_OriginBounding->Transform(*m_Bounding, WorldMatrix);
}

void CBoxCollision::SetCollision(_float3 vCenter, _float3 vExtents)
{
    m_OriginBounding->Center = vCenter;
    m_OriginBounding->Extents = vExtents;
}

_bool CBoxCollision::Intersect(COLLISION_TYPE eType, CCollision* pTarget)
{
    _bool bIsHit = false;

    if (m_bIsOnlyHitCollision)
    {
        if (!(m_bIsOnlyHitActorHashCode == typeid((*pTarget->GetOwner())).hash_code()))
            return false;
    }
    else
    {
        if (!IntersectAble(typeid((*pTarget->GetOwner())).hash_code()))
            return false;
    }

    switch (eType)
    {
    case COLLISION_TYPE::BOX:
        bIsHit = m_Bounding->Intersects(static_cast<CBoxCollision*>(pTarget)->GetBounding());
        break;
    case COLLISION_TYPE::SPHERE:
        bIsHit = m_Bounding->Intersects(static_cast<CSphereCollision*>(pTarget)->GetBounding());
        break;
    case COLLISION_TYPE::OBB:
        bIsHit = m_Bounding->Intersects(static_cast<COBBCollision*>(pTarget)->GetBounding());
        break;
    }

    return bIsHit;
}

_bool CBoxCollision::RayIntersect(COLLISION_TYPE eType, CCollision* pTarget, DEFAULT_HIT_DESC& OutDesc)
{
    _bool bIsHit = false;

    _float3 OwnerPosition = m_pOwner->GetTransform()->GetPosition();
    _float3 TargetPosition = pTarget->GetOwner()->GetTransform()->GetPosition();

    _vector CalCulationOwnerPosition = XMLoadFloat3(&OwnerPosition);
    _vector CalCulationTargetPosition = XMLoadFloat3(&TargetPosition);
    _vector vDireaction = XMVector3Normalize(CalCulationTargetPosition - CalCulationOwnerPosition);

    _float fDistance = { -1 };
    switch (eType)
    {
    case COLLISION_TYPE::BOX:
        bIsHit = m_Bounding->Intersects(CalCulationOwnerPosition, vDireaction, fDistance);
        break;
    case COLLISION_TYPE::SPHERE:
        bIsHit = m_Bounding->Intersects(CalCulationOwnerPosition, vDireaction, fDistance);
        break;
    case COLLISION_TYPE::OBB:
        bIsHit = m_Bounding->Intersects(CalCulationOwnerPosition, vDireaction, fDistance);
        break;
    }

    XMStoreFloat3(&OutDesc.vHitPoint, CalCulationOwnerPosition + vDireaction * fDistance);
    OutDesc.vfDistance = fDistance;
    XMStoreFloat3(&OutDesc.vDireaction, vDireaction);
    XMStoreFloat3(&OutDesc.vNormal, XMVector3Normalize(XMLoadFloat3(&OutDesc.vHitPoint) - CalCulationTargetPosition));
    return bIsHit;
}

_bool CBoxCollision::RayHit(_vector vOrizin, _vector vDiraction, DEFAULT_HIT_DESC& OutDesc)
{
    _bool bIsHit = false;
    _float fDistance = {};
    bIsHit = m_Bounding->Intersects(vOrizin, vDiraction, fDistance);

    XMStoreFloat3(&OutDesc.vHitPoint, vOrizin + vDiraction * fDistance);
    OutDesc.vfDistance = fDistance;
    XMStoreFloat3(&OutDesc.vDireaction, vDiraction);
    XMStoreFloat3(&OutDesc.vNormal, XMVector3Normalize(XMLoadFloat3(&OutDesc.vHitPoint) - vOrizin));
    return bIsHit;
}

ContainmentType CBoxCollision::Contains(_vector vPoint)
{
    return m_Bounding->Contains(vPoint);
}

void CBoxCollision::Render(_vector vColor)
{
    __super::Render(vColor);
    m_pBatch->Begin();

    DX::Draw(m_pBatch, *m_Bounding, false == m_bIsHit ? XMVectorSet(0.f, 1.f, 0.f, 1.f) : XMVectorSet(1.f, 0.f, 0.f, 1.f));

    m_pBatch->End();
}

CBoxCollision* CBoxCollision::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBoxCollision* pBoxCollision = new CBoxCollision(pDevice, pContext);
    if (FAILED(pBoxCollision->Initialize_Prototype()))
    {
        Safe_Release(pBoxCollision);
        MSG_BOX("CREATE FAIL : BOX COLLISION");
    }
    return pBoxCollision;
}

CComponent* CBoxCollision::Clone(void* pArg)
{
    CBoxCollision* pBoxCollision = new CBoxCollision(*this);
    if (FAILED(pBoxCollision->Initialize(pArg)))
    {
        Safe_Release(pBoxCollision);
        MSG_BOX("CLONE FAIL : BOX COLLISION");
    }
    return pBoxCollision;
}

void CBoxCollision::Free()
{
    __super::Free();

    Safe_Delete(m_OriginBounding);
    Safe_Delete(m_Bounding);
}