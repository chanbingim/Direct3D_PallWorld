#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)

class CPellBase;
class CPlayer;

class CPellPatrolState : public CState
{
public :
	enum PELL_MOVE_TYPE { WALK, SPRINT, END };

public :
	typedef struct	PellPatrolStateDesc
	{
		const CPellBase*			pActPell;
		CPlayer*					pPlayer;

		_float*						fPellMoveSpeed;
		_bool						bIsPartnerPell = false;
		_bool						bIsCombat = false;
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

	_bool								m_bIsPartnerPell = false;
	_float*								m_pPellMoveSpeed = nullptr;
	CPlayer*							m_pPlayer = nullptr;

	PELL_MOVE_TYPE						m_eMoveType = { PELL_MOVE_TYPE::END };

public:
	static CPellPatrolState*	Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END

