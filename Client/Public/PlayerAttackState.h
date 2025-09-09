#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPlayerAttackState : public CState
{
protected:
	CPlayerAttackState(const char* szStateName);
	virtual ~CPlayerAttackState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr)		override;
	virtual void OnStateExcution(void* pArg = nullptr)	override;
	virtual void OnStateExit(void* pArg = nullptr)		override;

private:
	_uInt						m_iAttackIndex = 0;

private:
	void						ChangeStateName();

public:
	static CPlayerAttackState*			Create(const char* szStateName);
	virtual void						Free() override;
};

NS_END