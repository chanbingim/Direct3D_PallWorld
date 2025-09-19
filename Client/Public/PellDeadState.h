#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPellDeadState : public CState
{
private:
	CPellDeadState(const char* szStateName);
	virtual ~CPellDeadState() = default;

public:
	virtual void				OnStateEnter(void* pArg = nullptr) override;
	virtual void				OnStateExcution(void* pArg = nullptr) override;
	virtual void				OnStateExit(void* pArg = nullptr) override;

public:
	static CPellDeadState*		Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END