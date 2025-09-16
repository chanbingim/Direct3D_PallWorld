#include "OBBCollision.h"
#include "BoxCollision.h"
#include "SphereCollision.h"

#include "DebugDraw.h"

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
    m_OriginOrientBox = new BoundingOrientedBox(Desc->vCneter, Desc->vExtents, Desc->vAngles);
    m_OrienteBox = new BoundingOrientedBox(*m_OriginOrientBox);

    return S_OK;
}

void COBBCollision::UpdateColiision(_matrix WorldMatrix)
{
    m_OriginOrientBox->Transform(*m_OrienteBox, WorldMatrix);
}

_bool COBBCollision::Intersect(COLLISION_TYPE eType, CCollision* pTarget)
{
    m_bIsHit = false;
    switch (eType)
    {
    case Engine::COLLISION_TYPE::BOX:
        m_bIsHit = m_OrienteBox->Intersects(static_cast<CBoxCollision*>(pTarget)->GetBounding());
        break;
    case Engine::COLLISION_TYPE::SPHERE:
        m_bIsHit = m_OrienteBox->Intersects(static_cast<CSphereCollision*>(pTarget)->GetBounding());
        break;
    case Engine::COLLISION_TYPE::OBB:
        m_bIsHit = m_OrienteBox->Intersects(static_cast<COBBCollision*>(pTarget)->GetBounding());
        break;
    }

    return m_bIsHit;
}

void COBBCollision::Render(_vector vColor)
{
    m_pBatch->Begin();

    DX::Draw(m_pBatch, *m_OrienteBox, vColor);

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
