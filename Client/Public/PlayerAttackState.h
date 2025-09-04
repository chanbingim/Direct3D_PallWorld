#pragma once
#include "State.h"

NS_BEGIN(Client)
class CPlayerAttackState : public CState
{
protected:
	CPlayerAttackState();
	virtual ~CPlayerAttackState() = default;

public:
	virtual void	OnEnterState(void* pArg = nullptr) override;
	virtual void	PlayState(void* pArg = nullptr) override;
	virtual void	OnEndState(void* pArg = nullptr) override;

public:
	static CPlayerAttackState*	Create();
	virtual void				Free() override;
};
NS_END