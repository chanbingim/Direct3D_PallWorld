#include "BoxCollision.h"
#include "SphereCollision.h"
#include "OBBCollision.h"

#include "DebugDraw.h"

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
    WorldMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[0]);
    WorldMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[1]);
    WorldMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(WorldMatrix.r[2]);

    m_OriginBounding->Transform(*m_Bounding, WorldMatrix);
}

_bool CBoxCollision::Intersect(COLLISION_TYPE eType, CCollision* pTarget)
{
    m_bIsHit = false;
    switch (eType)
    {
    case Engine::COLLISION_TYPE::BOX:
        m_bIsHit = m_Bounding->Intersects(static_cast<CBoxCollision*>(pTarget)->GetBounding());
        break;
    case Engine::COLLISION_TYPE::SPHERE:
        m_bIsHit = m_Bounding->Intersects(static_cast<CSphereCollision*>(pTarget)->GetBounding());
        break;
    case Engine::COLLISION_TYPE::OBB:
        m_bIsHit = m_Bounding->Intersects(static_cast<COBBCollision*>(pTarget)->GetBounding());
        break;
    }

    return m_bIsHit;
}

void CBoxCollision::Render(_vector vColor)
{
    m_pBatch->Begin();

    DX::Draw(m_pBatch, *m_Bounding, vColor);

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