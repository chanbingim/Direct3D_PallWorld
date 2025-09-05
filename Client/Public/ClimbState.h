#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CClimbState : public CState
{
private:
	CClimbState(const char* szStateName);
	virtual ~CClimbState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

public:
	static CClimbState*			Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END