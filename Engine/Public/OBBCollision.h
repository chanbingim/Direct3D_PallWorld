#pragma once
#include "Collision.h"

NS_BEGIN(Engine)
class ENGINE_DLL COBBCollision : public CCollision
{
public:
	typedef struct OBBColiisionDesc : public COLLISION_DESC
	{
		_float3			vExtents;
		_float4			vAngles;
	}OBB_COLLISION_DESC;

private:
	COBBCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	COBBCollision(const COBBCollision& rhs);
	virtual ~COBBCollision() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					UpdateColiision(_matrix WorldMatrix) override;
	virtual _bool					Intersect(COLLISION_TYPE eType, CCollision* pTarget) override;
	virtual _bool					RayIntersect(COLLISION_TYPE eType, CCollision* pTarget, DEFAULT_HIT_DESC& OutDesc) override;

#ifdef _DEBUG
	virtual void					Render(_vector vColor) override;
#endif // _DBUG

	const 	BoundingOrientedBox&	GetBounding() { return *m_OrienteBox; }

private:
	BoundingOrientedBox*					m_OriginOrientBox = nullptr;
	BoundingOrientedBox*					m_OrienteBox = nullptr;

public:
	static		COBBCollision*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*				Clone(void* pArg) override;
	virtual		void					Free()  override;

};
NS_END