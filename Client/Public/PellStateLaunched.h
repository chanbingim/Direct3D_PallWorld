#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Engine)
class CNavigation;
NS_END

NS_BEGIN(Client)
class CPellBase;

class CPellStateLaunched : public CState
{
public :
	typedef struct PellStateLaunchedDesc
	{
		const CPellBase*		pActPell;
		_float3					vDir;
		_float					fMoveSpeed;

		CNavigation*			pNavigation;
	}PELL_STATE_LAUNCHED_DESC;

private:
	CPellStateLaunched(const char* szStateName);
	virtual ~CPellStateLaunched() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

private:
	const CPellBase*					m_pActPell = nullptr;
	CNavigation*						m_pNavigation = nullptr;

	_float3								m_vDir = {};
	_float								m_fMoveSpeed = {};

public:
	static CPellStateLaunched*			Create(const char* szStateName);
	virtual void						Free() override;

};
NS_END