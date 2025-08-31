#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CCollision : public CComponent 
{
public :
	typedef struct CollisionDesc
	{
		CGameObject*			pOwner;
	}COLLISION_DESC;

protected :
	CCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollision(const CCollision& rhs);
	virtual ~CCollision() = default;

public :
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);

	void					BindBeginOverlapEvent(function<void(_float3 vDir, CGameObject* pHitActor)> BeginEvent);
	void					BindOverlappingEvent(function<void(_float3 vDir, CGameObject* pHitActor)> OverlappingEvent);
	void					BindEndOverlapEvent(function<void(_float3 vDir, CGameObject* pHitActor)> EndEvent);
	void					ADD_HitObejct(CGameObject* pObject);

	void					UpdateColiision();

protected:
	_bool						m_bIsHit = false;

	CGameObject*				m_pOwner = nullptr;
	list<CGameObject*>			m_OldHitList = {};
	list<CGameObject*>			m_HitList = {};
	
	function<void(_float3 vDir, CGameObject* pHitActor)> m_BeginHitFunc = nullptr;
	function<void(_float3 vDir, CGameObject* pHitActor)> m_OverlapHitFunc = nullptr;
	function<void(_float3 vDir, CGameObject* pHitActor)> m_EndHitFunc = nullptr;

public :
	virtual		CComponent*		Clone(void* pArg) override;	
	virtual		void			Free()  override;

};
NS_END