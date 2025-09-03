#pragma once
#include "State.h"

NS_BEGIN(Client)
class CPlayerSprintState : public CState
{
protected:
	CPlayerSprintState();
	virtual ~CPlayerSprintState() = default;

public:
	virtual void	OnEnterState(void* pArg = nullptr) override;
	virtual void	PlayState(void* pArg = nullptr) override;
	virtual void	OnEndState(void* pArg = nullptr) override;

public:
	static CPlayerSprintState*		Create();
	virtual void				Free() override;
};
NS_END