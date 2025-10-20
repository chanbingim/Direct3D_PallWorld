#pragma once
#include "Client_Define.h"
#include "Component.h"

NS_BEGIN(Engine)
class CCollision;
class CGameObject;
NS_END

NS_BEGIN(Client)
class CAiSenceComponent : public CComponent
{
public :
	typedef struct AiSenceComDesc
	{
		CGameObject*			pOwner;

		_float					fAiSearchRadius;
		_float					fAiTargetSearchDistance;
		_float					fAiTargetLostDistance;

	}AI_SENCE_COMPONENT_DESC;

private :
	CAiSenceComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAiSenceComponent(const CAiSenceComponent& Prototype);
	virtual ~CAiSenceComponent() = default;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(void* pArg) override;

	void								UpdatSenceComponent(_float fDeletaTime);

	void								Bind_TargetSearch(function<void(CGameObject*)> SearchFunc);
	void								Bind_TargetLost(function<void(CGameObject*)> TargetLostFunc);

private :
	CGameObject*						m_pOwner = nullptr;
	CCollision*							m_pTargetSearchCol = nullptr;

	function<void(CGameObject*)>		m_SearchFunc = nullptr;
	function<void(CGameObject*)>		m_TargetLostFunc = nullptr;

	set<CGameObject*>					m_pSearchList = {};

private :
	_float								m_fAiSearchRadius = {};
	_float								m_fAiTargetSearchDistance = {};
	_float								m_fAiTargetLostDistance = {};

private :
	HRESULT								ADD_Components();

	void								TargetSearch(_float3 vDir, CGameObject* pHitObject);
	void								TargetLost(_float3 vDir, CGameObject* pHitObject);

public :
	static		CAiSenceComponent*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*				Clone(void* pArg) override;
	virtual		void					Free() override;

};
NS_END