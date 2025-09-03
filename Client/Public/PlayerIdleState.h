#pragma once
#include "State.h"

NS_BEGIN(Client)
class CPlayerIdleState : public CState
{
protected:
	CPlayerIdleState();
	virtual ~CPlayerIdleState() = default;

public:
	virtual void	OnEnterState(void* pArg = nullptr) override;
	virtual void	PlayState(void* pArg = nullptr) override;
	virtual void	OnEndState(void* pArg = nullptr) override;

public:
	static CPlayerIdleState*	Create();
	virtual void				Free() override;
};
NS_END