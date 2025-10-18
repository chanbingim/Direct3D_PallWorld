#pragma once
#include "Collision.h"

NS_BEGIN(Engine)
class ENGINE_DLL CSphereCollision : public CCollision
{
public :
	typedef struct SphereColiisionDesc : public COLLISION_DESC
	{
		_float			Radius;
	}SPEHRE_COLLISION_DESC;

private :
	CSphereCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSphereCollision(const CSphereCollision& rhs);
	virtual ~CSphereCollision() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					UpdateColiision(_matrix WorldMatrix) override;
	
	virtual _bool					Intersect(COLLISION_TYPE eType, CCollision* pTarget) override;
	virtual _bool					RayIntersect(COLLISION_TYPE eType, CCollision* pTarget, DEFAULT_HIT_DESC& OutDesc) override;
	virtual _bool					RayHit(_vector vOrizin, _vector vDiraction, DEFAULT_HIT_DESC& OutDesc) override;
	virtual	ContainmentType			Contains(_vector vPoint) override;

	void							SetCollision(_float3 vCenter, _float fRadius);

#ifdef _DEBUG
	virtual void					Render(_vector vColor) override;
#endif // _DBUG

	const 	BoundingSphere&			GetBounding() { return *m_Bounding; }

private :
	BoundingSphere*					m_OriginBounding = nullptr;
	BoundingSphere*					m_Bounding = nullptr;

public:
	static		CSphereCollision*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*			Clone(void* pArg) override;
	virtual		void				Free()  override;

};
NS_END