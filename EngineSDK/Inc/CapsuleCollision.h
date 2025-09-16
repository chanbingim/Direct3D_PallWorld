#pragma once
#include "Collision.h"

NS_BEGIN(Engine)
class ENGINE_DLL CCapsuleCollision : public CCollision
{
public:
	typedef struct CapsuleColiisionDesc : public COLLISION_DESC
	{
	}CAPSULE_COLLISION_DESC;

private:
	CCapsuleCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCapsuleCollision(const CCapsuleCollision& rhs);
	virtual ~CCapsuleCollision() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);

private:

public:
	virtual		CCapsuleCollision*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*			Clone(void* pArg) override;
	virtual		void				Free()  override;
};
NS_END