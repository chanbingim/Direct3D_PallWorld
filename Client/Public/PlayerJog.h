#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPlayerJogState : public CState
{
private:
	CPlayerJogState(const char* szStateName);
	virtual ~CPlayerJogState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

public:
	static CPlayerJogState*		Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END