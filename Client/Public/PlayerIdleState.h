#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPlayerIdleState : public CState
{
private :
	CPlayerIdleState(const char* szStateName);
	virtual ~CPlayerIdleState() = default;

public :
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

public :
	static CPlayerIdleState*	Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END