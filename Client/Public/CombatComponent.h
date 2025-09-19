#pragma once
#include "Client_Define.h"
#include "Component.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)
class CCombatComponent final : public CComponent
{
public :
	typedef struct CombatCom_Desc
	{
		CGameObject*					pOwner;
		_float							fLostTargetTime;
		_float							fChangeTargetDistance;

		function<void(CGameObject*)>	CallBackFunction;
	}COMBAT_COMPONENT_DESC;

private :
	CCombatComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCombatComponent() = default;

public :
	HRESULT										Initialzie(void* pArg);
	void										Update(_float fDeletaTime);

	void										UpdateTarget();
	void										ADD_TargetObject(CGameObject* pTarget);

private :
	CGameObject*								m_pOwner = nullptr;
	list<CGameObject*>							m_pTargetList = {};
	CGameObject*								m_pTargetObject = {};

	// 전투 진행시 호출해줄 녀석의 함수
	function<void(CGameObject*)>				m_BindCombatFunc = nullptr;

	_float										m_fLostTargetTime = {};
	_float										m_fChangeTargetDistance = {};

public :
	static			CCombatComponent*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent*									Clone(void* pArg) override;
	virtual			void						Free() override;

};
NS_END