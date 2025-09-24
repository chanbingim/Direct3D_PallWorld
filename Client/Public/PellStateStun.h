#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CPellStateStun : public CState
{
public :
	typedef struct PellStateStunDesc
	{
		function<void(_float fDeletaTime, _bool bIsEndStun)>	ActionFunction;
		_float													StunTime;
	}PELL_STATE_STUN_STATE;

private:
	CPellStateStun(const char* szStateName);
	virtual ~CPellStateStun() = default;

public:
	virtual void				OnStateEnter(void* pArg = nullptr) override;
	virtual void				OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void				OnStateExit(void* pArg = nullptr) override;

private :
	function<void(_float fDeletaTime, _bool bIsEndStun)>	m_ActionFunc = nullptr;

	_bool									m_bIsStun = false;
	_float									m_fAccStunTime = {};
	_float									m_fEndStunTime = {};

public:
	static CPellStateStun*		Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END
