#include "SphereCollision.h"
#include "OBBCollision.h"
#include "BoxCollision.h"

#include "DebugDraw.h"

CSphereCollision::CSphereCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CCollision(pDevice, pContext)
{
}

CSphereCollision::CSphereCollision(const CSphereCollision& rhs) :
	CCollision(rhs)
{
}

HRESULT CSphereCollision::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	m_CollisionType = COLLISION_TYPE::SPHERE;
	return S_OK;
}

HRESULT CSphereCollision::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	SPEHRE_COLLISION_DESC* Desc = static_cast<SPEHRE_COLLISION_DESC*>(pArg);
	m_OriginBounding = new BoundingSphere(Desc->vCneter, Desc->Radius);
	m_Bounding = new BoundingSphere(*m_OriginBounding);

	return S_OK;
}

void CSphereCollision::UpdateColiision(_matrix WorldMatrix)
{
	m_OriginBounding->Transform(*m_Bounding, WorldMatrix);
}

_bool CSphereCollision::Intersect(COLLISION_TYPE eType, CCollision* pTarget)
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

void CSphereCollision::Render(_vector vColor)
{
	m_pBatch->Begin();

	DX::Draw(m_pBatch, *m_Bounding, vColor);

	m_pBatch->End();
}

CSphereCollision* CSphereCollision::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSphereCollision* pSphereCollision = new CSphereCollision(pDevice, pContext);
	if (FAILED(pSphereCollision->Initialize_Prototype()))
	{
		Safe_Release(pSphereCollision);
		MSG_BOX("CREATE FAIL : SPEHRE COLLISION");
	}
	return pSphereCollision;
}

CComponent* CSphereCollision::Clone(void* pArg)
{
	CSphereCollision* pSphereCollision = new CSphereCollision(*this);
	if (FAILED(pSphereCollision->Initialize(pArg)))
	{
		Safe_Release(pSphereCollision);
		MSG_BOX("CLONE FAIL : SPEHRE COLLISION");
	}
	return pSphereCollision;
}

void CSphereCollision::Free()
{
	__super::Free();

	Safe_Delete(m_OriginBounding);
	Safe_Delete(m_Bounding);
}
