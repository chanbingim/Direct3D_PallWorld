#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPlayer; 

class CJumpState : public CState
{
private :
	CJumpState(const char* szStateName);
	virtual ~CJumpState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

private :
	_uInt						m_iJumpIndex = 0;

private :
	void						ChangeStateName();

public:
	static CJumpState*			Create(const char* szStateName);
	virtual void				Free() override;
	
};
NS_END