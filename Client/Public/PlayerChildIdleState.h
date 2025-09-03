#pragma once
#include "State.h"

NS_BEGIN(Client)
class CPlayerChildIdleState : public CState
{
protected:
	CPlayerChildIdleState();
	virtual ~CPlayerChildIdleState() = default;

public:
	virtual void	OnEnterState(void* pArg = nullptr) override;
	virtual void	PlayState(void* pArg = nullptr) override;
	virtual void	OnEndState(void* pArg = nullptr) override;

public:
	static CPlayerChildIdleState*	Create();
	virtual void					Free() override;
};
NS_END