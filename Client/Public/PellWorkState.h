#pragma once
#include "Client_Define.h"
#include "PellStructData.h"
#include "State.h"

NS_BEGIN(Engine)
class CNavigation;
class CGameObject;
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CPellBase;
class CWorkComponent;
class CPellBoxManager;

class CPellWorkState : public CState
{
public :
	typedef struct PellWorkStateDesc
	{
		const CPellBase*				pActPell;
		CGameObject*					pTargetObject;
		CNavigation*					pNavigation;

		function<void()>				WorkEndFunc;
	}PELL_WORK_STATE_DESC;

private:
	CPellWorkState(const char* szStateName);
	virtual ~CPellWorkState() = default;

public:
	HRESULT						Initialize(CPellBase* pOwner);
	virtual void				OnStateEnter(void* pArg = nullptr) override;
	virtual void				OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void				OnStateExit(void* pArg = nullptr) override;

private :
	CGameInstance*				m_pGameInstance = nullptr;
	CPellBoxManager*			m_pPellBoxManager = nullptr;
	const CPellBase*			m_pActPell = nullptr;

	CWorkComponent*				m_pWorkComponent = nullptr;
	CGameObject*				m_pJobTarget = nullptr;
	CNavigation*				m_pNavigation = nullptr;

	function<void()>			m_WorkEndFunc = nullptr;
	PELL_WORK_TYPE				m_eWorkType = {};

	_bool						m_bNext = false;
private :
	void						UpdateWorkName();

public:
	static CPellWorkState*		Create(const char* szStateName, CPellBase* pOwner);
	virtual void				Free() override;
};
NS_END