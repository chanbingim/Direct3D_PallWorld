#pragma once
#include "Collision.h"

NS_BEGIN(Engine)
class ENGINE_DLL CSphereCollision : public CCollision
{
public :
	typedef struct SphereColiisionDesc : public COLLISION_DESC
	{
		float			Radius;
	}SPEHRE_COLLISION_DESC;

private :
	CSphereCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSphereCollision(const CSphereCollision& rhs);
	virtual ~CSphereCollision() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);

	const 	BoundingSphere&			GetBounding() { return m_Bounding; }

private :
	BoundingSphere					m_Bounding;

public:
	virtual		CSphereCollision*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*			Clone(void* pArg) override;
	virtual		void				Free()  override;

};
NS_END