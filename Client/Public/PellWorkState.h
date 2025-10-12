#pragma once
#include "Client_Define.h"
#include "PellStructData.h"
#include "State.h"

NS_BEGIN(Engine)
class CNavigation;
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CPellBase;
class CPellBoxManager;

class CPellWorkState : public CState
{
public :
	typedef struct PellWorkStateDesc
	{
		const CPellBase*				pActPell;
		CNavigation*					pNavigation;
	}PELL_WORK_STATE_DESC;

private:
	CPellWorkState(const char* szStateName);
	virtual ~CPellWorkState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

private :
	CGameInstance*				m_pGameInstance = nullptr;
	CPellBoxManager*			m_pPellBoxManager = nullptr;

	const CPellBase*			m_pActPell = nullptr;
	CNavigation*				m_pNavigation = nullptr;

	PELL_WORK_TYPE				m_eWorkType = {};
	_bool						m_bIsGoToWork = false;

private :
	void						UpdateWorkName();

	// 이건 그냥 작업을 선택하고 이동하는 과정이라 보면됨
	void						GoToWork();

	// 가장 가까운 작업을 우순선위로 한다.
	void						PellWork();

public:
	static CPellWorkState*		Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END