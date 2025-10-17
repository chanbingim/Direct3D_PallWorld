#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPellIdleState : public CState	
{
private:
	CPellIdleState(const char* szStateName);
	virtual ~CPellIdleState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

public:
	static CPellIdleState*		Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END