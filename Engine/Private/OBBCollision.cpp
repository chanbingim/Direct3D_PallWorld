#include "OBBCollision.h"
#include "BoxCollision.h"
#include "SphereCollision.h"

#include "Transform.h"

#include "DebugDraw.h"
#include "GameObject.h"

COBBCollision::COBBCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CCollision(pDevice, pContext)
{
}

COBBCollision::COBBCollision(const COBBCollision& rhs) :
    CCollision(rhs)
{
}

HRESULT COBBCollision::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    m_CollisionType = COLLISION_TYPE::OBB;
    return S_OK;
}

HRESULT COBBCollision::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    OBB_COLLISION_DESC* Desc = static_cast<OBB_COLLISION_DESC*>(pArg);

    _float4		vQuaternion = {};
    XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYaw(Desc->vAngles.x, Desc->vAngles.y, Desc->vAngles.z));

    m_OriginOrientBox = new BoundingOrientedBox(Desc->vCneter, Desc->vExtents, vQuaternion);
    m_OrienteBox = new BoundingOrientedBox(*m_OriginOrientBox);

    return S_OK;
}

void COBBCollision::UpdateColiision(_matrix WorldMatrix)
{
    m_bIsHit = false;
    m_OriginOrientBox->Transform(*m_OrienteBox, WorldMatrix);
}

_bool COBBCollision::Intersect(COLLISION_TYPE eType, CCollision* pTarget)
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
        bIsHit = m_OrienteBox->Intersects(static_cast<CBoxCollision*>(pTarget)->GetBounding());
        break;
    case COLLISION_TYPE::SPHERE:
        bIsHit = m_OrienteBox->Intersects(static_cast<CSphereCollision*>(pTarget)->GetBounding());
        break;
    case COLLISION_TYPE::OBB:
        bIsHit = m_OrienteBox->Intersects(static_cast<COBBCollision*>(pTarget)->GetBounding());
        break;
    }

    return bIsHit;
}

_bool COBBCollision::RayIntersect(COLLISION_TYPE eType, CCollision* pTarget, DEFAULT_HIT_DESC& OutDesc)
{
    _bool bIsHit = false;

    _float3 OwnerPosition = m_pOwner->GetTransform()->GetPosition();
    _float3 TargetPosition = pTarget->GetOwner()->GetTransform()->GetPosition();

    _vector CalCulationOwnerPosition = XMLoadFloat3(&OwnerPosition);
    _vector CalCulationTargetPosition = XMLoadFloat3(&TargetPosition);
    _vector vDireaction = XMVector3Normalize(CalCulationTargetPosition - CalCulationOwnerPosition);

    _float fDistance = {};
    switch (eType)
    {
    case COLLISION_TYPE::BOX:
        bIsHit = m_OrienteBox->Intersects(CalCulationOwnerPosition, vDireaction, fDistance);
        break;
    case COLLISION_TYPE::SPHERE:
        bIsHit = m_OrienteBox->Intersects(CalCulationOwnerPosition, vDireaction, fDistance);
        break;
    case COLLISION_TYPE::OBB:
        bIsHit = m_OrienteBox->Intersects(CalCulationOwnerPosition, vDireaction, fDistance);
        break;
    }

    XMStoreFloat3(&OutDesc.vHitPoint, CalCulationOwnerPosition + vDireaction * fDistance);
    XMStoreFloat3(&OutDesc.vDireaction, vDireaction);
    XMStoreFloat3(&OutDesc.vNormal, XMVector3Normalize(XMLoadFloat3(&OutDesc.vHitPoint) - CalCulationTargetPosition));
    return bIsHit;
}

void COBBCollision::Render(_vector vColor)
{
    __super::Render(vColor);
    m_pBatch->Begin();

    DX::Draw(m_pBatch, *m_OrienteBox, false == m_bIsHit ? XMVectorSet(0.f, 1.f, 0.f, 1.f) : XMVectorSet(1.f, 0.f, 0.f, 1.f));

    m_pBatch->End();
}

COBBCollision* COBBCollision::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    COBBCollision* pOBBColiision = new COBBCollision(pDevice, pContext);
    if (FAILED(pOBBColiision->Initialize_Prototype()))
    {
        Safe_Release(pOBBColiision);
        MSG_BOX("CREATE FAIL : OBB COLLISION");
    }
    return pOBBColiision;
}

CComponent* COBBCollision::Clone(void* pArg)
{
    COBBCollision* pOBBColiision = new COBBCollision(*this);
    if (FAILED(pOBBColiision->Initialize(pArg)))
    {
        Safe_Release(pOBBColiision);
        MSG_BOX("CLONE FAIL : OBB COLLISION");
    }
    return pOBBColiision;
}

void COBBCollision::Free()
{
    __super::Free();
    Safe_Delete(m_OriginOrientBox);
    Safe_Delete(m_OrienteBox);
}
