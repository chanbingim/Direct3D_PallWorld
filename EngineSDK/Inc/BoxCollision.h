#pragma once
#include "Collision.h"

NS_BEGIN(Engine)
class ENGINE_DLL CBoxCollision : public CCollision
{
public:
	typedef struct BoxColiisionDesc : public COLLISION_DESC
	{
		_float3			Extents;
	}BOX_COLLISION_DESC;

private:
	CBoxCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoxCollision(const CBoxCollision& rhs);
	virtual ~CBoxCollision() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					UpdateColiision(_matrix WorldMatrix) override;
	void							SetCollision(_float3 vCenter, _float3 vExtents);

	virtual _bool					Intersect(COLLISION_TYPE eType, CCollision* pTarget) override;
	virtual _bool					RayIntersect(COLLISION_TYPE eType, CCollision* pTarget, DEFAULT_HIT_DESC& OutDesc) override;
	virtual _bool					RayHit(_vector vOrizin, _vector vDiraction, DEFAULT_HIT_DESC& OutDesc) override;
	virtual	ContainmentType			Contains(_vector vPoint) override;

#ifdef _DEBUG
	virtual void					Render(_vector vColor) override;
#endif // _DBUG

	const 	BoundingBox&			GetBounding() { return *m_Bounding; }

private:
	BoundingBox*					m_OriginBounding = nullptr;
	BoundingBox*					m_Bounding = nullptr;

public:
	static		CBoxCollision*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*			Clone(void* pArg) override;
	virtual		void				Free()  override;
};
NS_END