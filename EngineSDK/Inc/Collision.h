#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CCollision abstract : public CComponent 
{
public:
	typedef struct CollisionDesc
	{
		CGameObject* pOwner;
		_float3					vCneter;
	}COLLISION_DESC;

	typedef struct DefaultHitDesc
	{
		_float3					vHitPoint;
		_float					vfDistance;
		_float3					vDireaction;
		_float3					vNormal;
	}DEFAULT_HIT_DESC;

protected:
	CCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollision(const CCollision& rhs);
	virtual ~CCollision() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			UpdateColiision(_matrix WorldMatrix);
	
	virtual _bool			Intersect(COLLISION_TYPE eType, CCollision* pTarget) = 0;
	virtual _bool			RayIntersect(COLLISION_TYPE eType, CCollision* pTarget, DEFAULT_HIT_DESC& OutDesc) { return false; }
	virtual _bool			RayHit(_vector vOrizin, _vector vDiraction, DEFAULT_HIT_DESC& OutDesc);
	
	
	virtual void			Render(_vector vColor = {1.f, 0.f, 0.f, 1.f});

	void					BindBeginOverlapEvent(function<void(_float3 vDir, CGameObject* pHitActor)> BeginEvent);
	void					BindOverlappingEvent(function<void(_float3 vDir, CGameObject* pHitActor)> OverlappingEvent);
	void					BindEndOverlapEvent(function<void(_float3 vDir, CGameObject* pHitActor)> EndEvent);

	void					ADD_HitObejct(CGameObject* pObject);
	void					ADD_IgnoreObejct(size_t typeID);
	void					ADD_OnlyHitObject(size_t typeID);

	void					CallFunction();

	const COLLISION_TYPE&	GetCollisionType() const { return m_CollisionType; }
	CGameObject*			GetOwner() { return m_pOwner; }

protected:
	COLLISION_TYPE				m_CollisionType = {};
	CGameObject*				m_pOwner = nullptr;
	_bool						m_bIsOnlyHitCollision = false;
	_bool						m_bIsHit = false;
	
	list<CGameObject*>			m_OldHitList = {};
	list<CGameObject*>			m_HitList = {};

	size_t						m_bIsOnlyHitActorHashCode = {};
	set<size_t>					m_IgnoreObject;
	

	function<void(_float3 vDir, CGameObject* pHitActor)> m_BeginHitFunc = nullptr;
	function<void(_float3 vDir, CGameObject* pHitActor)> m_OverlapHitFunc = nullptr;
	function<void(_float3 vDir, CGameObject* pHitActor)> m_EndHitFunc = nullptr;

#ifdef _DEBUG
	PrimitiveBatch<DirectX::VertexPositionColor>*		m_pBatch = { nullptr };
	BasicEffect*										m_pEffect = { nullptr };
	ID3D11InputLayout*									m_pInputLayout = { nullptr };
#endif // _DEBUG

protected :
	_bool						IntersectAble(size_t TagetTypeHashCode);

public :
	virtual		CComponent*		Clone(void* pArg) override;	
	virtual		void			Free()  override;

};
NS_END