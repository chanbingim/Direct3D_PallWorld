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
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);

	const 	BoundingBox&			GetBounding() { return m_Bounding; }

private:
	BoundingBox						m_Bounding;

public:
	virtual		CBoxCollision*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*			Clone(void* pArg) override;
	virtual		void				Free()  override;
};
NS_END