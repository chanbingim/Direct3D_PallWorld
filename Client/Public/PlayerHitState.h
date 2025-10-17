#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPlayer;

class CPlayerHitState : public CState
{
protected:
	CPlayerHitState(const char* szStateName);
	virtual ~CPlayerHitState() = default;

public:
	virtual void				OnStateEnter(void* pArg = nullptr)		override;
	virtual void				OnStateExcution(_float fDeletaTime, void* pArg = nullptr)	override;
	virtual void				OnStateExit(void* pArg = nullptr)		override;

	

private :
	CPlayer*					m_pPlayer = nullptr;

private:
	void						ChangeStateName();

public:
	static CPlayerHitState*				Create(const char* szStateName);
	virtual void						Free() override;

};
NS_END