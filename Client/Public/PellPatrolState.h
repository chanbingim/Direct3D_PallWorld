#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)

class CPellBase;

class CPellPatrolState : public CState
{
public :
	enum PELL_MOVE_TYPE { WALK, SPRINT, END };

public :
	typedef struct	PellPatrolStateDesc
	{
		
		const CPellBase*			pActPell;
		_float*						fPellMoveSpeed;

		_bool						bIsPartnerPell = false;
		PELL_MOVE_TYPE				ePellMoveType = { PELL_MOVE_TYPE::END };
	}PELL_PATROL_STATE_DESC;

private:
	CPellPatrolState(const char* szStateName);
	virtual ~CPellPatrolState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

private : 
	const CPellBase*					m_pActPell = nullptr;
	PELL_MOVE_TYPE						m_eMoveType = { PELL_MOVE_TYPE::END };

public:
	static CPellPatrolState*	Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END

