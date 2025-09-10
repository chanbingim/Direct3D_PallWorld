#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPellResetState : public CState
{
private:
	CPellResetState(const char* szStateName);
	virtual ~CPellResetState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

private :
	_uInt						m_iRandomRestMotion = {};

public:
	static CPellResetState*		Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END