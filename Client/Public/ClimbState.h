#pragma once
#include "State.h"

NS_BEGIN(Client)
class CClimbState : public CState
{
protected:
	CClimbState();
	virtual ~CClimbState() = default;

public:
	virtual void	OnEnterState(void* pArg = nullptr) override;
	virtual void	PlayState(void* pArg = nullptr) override;
	virtual void	OnEndState(void* pArg = nullptr) override;

public:
	static CClimbState*			Create();
	virtual void				Free() override;
};
NS_END