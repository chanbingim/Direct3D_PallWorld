#pragma once
#include "State.h"

NS_BEGIN(Client)
class CCrouchState : public CState
{
protected:
	CCrouchState();
	virtual ~CCrouchState() = default;

public:
	virtual void	OnEnterState(void* pArg = nullptr) override;
	virtual void	PlayState(void* pArg = nullptr) override;
	virtual void	OnEndState(void* pArg = nullptr) override;

public:
	static CCrouchState*		Create();
	virtual void				Free() override;
};
NS_END