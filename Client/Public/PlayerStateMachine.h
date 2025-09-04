#pragma once

#include "FiniteStateMachine.h"

NS_BEGIN(Client)
class CPlayerStateMachine final : public CFiniteStateMachine
{
public:
	enum class MOVE_ACTION { DEFAULT, CROUCH, CLIMB, JUMP, EMOTE, HIT, ATTACK,END };
	enum class MOVE_CHILD_ACTION { WALK, IDLE, JOG, SPRINT, END };
	enum class NONE_MOVE_ACTION { PETTING, CARRY, SLEEP, COOK, END };

	typedef	struct	PLAYER_STATE
	{
		// 현재 무기를 Player가 들고있는가
		_bool				bIsAiming;
		_bool				bIsAttacking;
		WEAPON				eWeaponType;

		// 이동 과 같이 나와야하는 상태들
		MOVE_ACTION			eMove_State;
		MOVE_CHILD_ACTION	eMove_Child_State;

		// 움직임을 가져가지 못하는 녀석들을 모아놓은 state
		NONE_MOVE_ACTION	eNone_Move_State;
	}PLAYER_STATE;

protected:
	CPlayerStateMachine();
	CPlayerStateMachine(const CPlayerStateMachine& rhs);
	virtual ~CPlayerStateMachine() = default;

public:
	virtual		HRESULT					Initialize(void* pArg = nullptr);
	virtual		void					Update(_float DeltaTime);

	const PLAYER_STATE&					GetState() { return m_StateData; }
	void								ChangeState(_uInt iStateID, _uInt iSTateIndex, const _wstring& StateTag);

	void								SetAiming(_bool	bFlag) { m_StateData.bIsAiming = bFlag; }
	void								SetWeapon(const WEAPON	eWeapon) { m_StateData.eWeaponType = eWeapon; }
	void								SetAttack(_bool	bFlag) { m_StateData.bIsAttacking = bFlag; }

	_string								GetStateFullName();

private : 
	char								m_FullName[MAX_PATH] = {};
	PLAYER_STATE						m_StateData = {};

	vector<_uInt>*						m_StatesIndex = nullptr;
	pair<_wstring, CState*>*			m_CurrentStates = nullptr;

private :
	HRESULT								ADD_PlayerMoveState(_uInt iIndex);
	HRESULT								ADD_PlayerChildState(_uInt iIndex);
	void								SettingPlayerState(void* pArg = nullptr);

public:
	static		CPlayerStateMachine*		Create();
	virtual		void						Free() override;

};
NS_END