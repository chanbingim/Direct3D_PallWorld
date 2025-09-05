#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPlayerSprintState : public CState
{
private:
	CPlayerSprintState(const char* szStateName);
	virtual ~CPlayerSprintState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

public:
	static CPlayerSprintState*	Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END