#pragma once
#include "Client_Define.h"
#include "Component.h"

NS_BEGIN(Engine)
class CActor;
class CNavigation;
NS_END

NS_BEGIN(Client)
class CArchitecture;

class CDefenceBossComponent : public CComponent
{
public :
	typedef struct DefenceBossComponentDesc
	{
		const	CActor*				pOwner;
		const	CArchitecture*		pTargetArchitecture;
		CNavigation*				pNavigation;
	}DEFENCE_BOSS_COMPONENT_DESC;

protected:
	CDefenceBossComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDefenceBossComponent(const CDefenceBossComponent& Prototype);
	virtual ~CDefenceBossComponent() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);

	virtual HRESULT			Update(_float	fDeletatime);

	HRESULT					ComputeTargetAcrchitectureAStar(list<_float3>* PathList);

private :
	const	CActor*						m_pOwner = nullptr;
	// 공격할 건물 목표
	const	CArchitecture*				m_pTargetArchitecture = nullptr;

	// 건물 목표로 이동하는 최단거리를 가져올 네비게이션
	CNavigation*						m_pNavigation = nullptr;

	list<_float3>						m_pPathList = {};
	_float								m_fMoveSpeed = {};

public:
	static	CDefenceBossComponent*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*					Clone(void* pArg) override;
	virtual void						Free();

};
NS_END