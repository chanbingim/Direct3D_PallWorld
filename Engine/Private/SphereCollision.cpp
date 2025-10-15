#include "SphereCollision.h"
#include "OBBCollision.h"
#include "BoxCollision.h"

#include "Transform.h"

#include "DebugDraw.h"
#include "GameObject.h"

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
	m_bIsHit = false;

	m_OriginBounding->Transform(*m_Bounding, WorldMatrix);
}

_bool CSphereCollision::Intersect(COLLISION_TYPE eType, CCollision* pTarget)
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

_bool CSphereCollision::RayIntersect(COLLISION_TYPE eType, CCollision* pTarget, DEFAULT_HIT_DESC& OutDesc)
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

_bool CSphereCollision::RayHit(_vector vOrizin, _vector vDiraction, DEFAULT_HIT_DESC& OutDesc)
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

void CSphereCollision::SetCollision(_float3 vCenter, _float fRadius)
{
	m_OriginBounding->Center = vCenter;
	m_OriginBounding->Radius = fRadius;
}

void CSphereCollision::Render(_vector vColor)
{
	__super::Render(vColor);
	m_pBatch->Begin();

	DX::Draw(m_pBatch, *m_Bounding, false == m_bIsHit ? XMVectorSet(0.f, 1.f, 0.f, 1.f) : XMVectorSet(1.f, 0.f, 0.f, 1.f));

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
