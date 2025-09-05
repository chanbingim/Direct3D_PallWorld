#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CCrouchState : public CState
{
private:
	CCrouchState(const char* szStateName);
	virtual ~CCrouchState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

public:
	static CCrouchState*		Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END