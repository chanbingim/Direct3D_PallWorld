#pragma once
#include "State.h"

NS_BEGIN(Client)
class CPlayerWalkState : public CState
{
protected:
	CPlayerWalkState();
	virtual ~CPlayerWalkState() = default;

public:
	virtual void	OnEnterState(void* pArg = nullptr) override;
	virtual void	PlayState(void* pArg = nullptr) override;
	virtual void	OnEndState(void* pArg = nullptr) override;

public:
	static CPlayerWalkState*	Create();
	virtual void				Free() override;
};
NS_END