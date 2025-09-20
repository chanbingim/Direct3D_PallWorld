#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPellPatrolState : public CState
{
private:
	CPellPatrolState(const char* szStateName);
	virtual ~CPellPatrolState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

public:
	static CPellPatrolState*	Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END

