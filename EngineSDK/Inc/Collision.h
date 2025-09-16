#pragma once
#include "Component.h"



NS_BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CCollision : public CComponent 
{
public :
	typedef struct CollisionDesc
	{
		_float3					vCneter;
	}COLLISION_DESC;

protected :
	CCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollision(const CCollision& rhs);
	virtual ~CCollision() = default;

public :
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			UpdateColiision(_matrix WorldMatrix);
	virtual _bool			Intersect(COLLISION_TYPE eType, CCollision* pTarget) = 0;
	virtual void			Render(_vector vColor);

	void					BindBeginOverlapEvent(function<void(_float3 vDir, CGameObject* pHitActor)> BeginEvent);
	void					BindOverlappingEvent(function<void(_float3 vDir, CGameObject* pHitActor)> OverlappingEvent);
	void					BindEndOverlapEvent(function<void(_float3 vDir, CGameObject* pHitActor)> EndEvent);
	void					ADD_HitObejct(CGameObject* pObject);
	void					CallFunction();

	const COLLISION_TYPE&	GetCollisionType() const { return m_CollisionType; }

protected:
	COLLISION_TYPE				m_CollisionType = {};
	_bool						m_bIsHit = false;

	list<CGameObject*>			m_OldHitList = {};
	list<CGameObject*>			m_HitList = {};
	
	function<void(_float3 vDir, CGameObject* pHitActor)> m_BeginHitFunc = nullptr;
	function<void(_float3 vDir, CGameObject* pHitActor)> m_OverlapHitFunc = nullptr;
	function<void(_float3 vDir, CGameObject* pHitActor)> m_EndHitFunc = nullptr;

#ifdef _DEBUG
	PrimitiveBatch<DirectX::VertexPositionColor>*		m_pBatch = { nullptr };
	BasicEffect*										m_pEffect = { nullptr };
	ID3D11InputLayout*									m_pInputLayout = { nullptr };
#endif // _DEBUG

public :
	virtual		CComponent*		Clone(void* pArg) override;	
	virtual		void			Free()  override;

};
NS_END