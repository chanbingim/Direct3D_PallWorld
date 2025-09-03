#pragma once
#include "State.h"

NS_BEGIN(Client)
class CHitState : public CState
{
protected:
	CHitState();
	virtual ~CHitState() = default;

public:
	virtual void	OnEnterState(void* pArg = nullptr) override;
	virtual void	PlayState(void* pArg = nullptr) override;
	virtual void	OnEndState(void* pArg = nullptr) override;

public:
	static CHitState*		Create();
	virtual void			Free() override;
};
NS_END