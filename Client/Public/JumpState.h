#pragma once
#include "State.h"

NS_BEGIN(Client)
class CJumpState : public CState
{
protected:
	CJumpState();
	virtual ~CJumpState() = default;

public:
	virtual void	OnEnterState(void* pArg = nullptr) override;
	virtual void	PlayState(void* pArg = nullptr) override;
	virtual void	OnEndState(void* pArg = nullptr) override;

public:
	static CJumpState*			Create();
	virtual void				Free() override;
	
};
NS_END