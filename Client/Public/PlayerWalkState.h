#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPlayerWalkState : public CState
{
private:
	CPlayerWalkState(const char* szStateName);
	virtual ~CPlayerWalkState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

public:
	static CPlayerWalkState*	Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END