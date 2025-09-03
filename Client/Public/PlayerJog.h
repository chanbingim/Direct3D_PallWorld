#pragma once
#include "State.h"

NS_BEGIN(Client)
class CPlayerJogState : public CState
{
protected:
	CPlayerJogState();
	virtual ~CPlayerJogState() = default;

public:
	virtual void	OnEnterState(void* pArg = nullptr) override;
	virtual void	PlayState(void* pArg = nullptr) override;
	virtual void	OnEndState(void* pArg = nullptr) override;

public:
	static CPlayerJogState*		Create();
	virtual void				Free() override;
};
NS_END