#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CNpcTalkState : public CState
{
private:
	CNpcTalkState(const char* szStateName);
	virtual ~CNpcTalkState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

private:
	_uInt							m_iTalkIndex = {};

public:
	static CNpcTalkState*			Create(const char* szStateName);
	virtual void					Free() override;
};
NS_END