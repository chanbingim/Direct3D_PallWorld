#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPellAttackState : public CState
{
private:
	CPellAttackState(const char* szStateName);
	virtual ~CPellAttackState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

public:
	static CPellAttackState*	Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END