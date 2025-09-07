#pragma once

#include "Client_Define.h"
#include "FiniteStateMachine.h"

NS_BEGIN(Client)
class CPlayerStateMachine final : public CFiniteStateMachine
{
public:
	enum class MOVE_ACTION { DEFAULT, CROUCH, CLIMB, JUMP, END };
	enum class MOVE_CHILD_ACTION { WALK, IDLE, JOG, SPRINT, END };
	enum class NONE_MOVE_ACTION { PETTING, CARRY, SLEEP, COOK, END };
	

	typedef	struct	PLAYER_STATE
	{
		// 현재 무기를 Player가 들고있는가
		_bool				bIsAiming;
		_bool				bIsAttacking;
		DIRECTION			eDireaction;
		_uInt				iWeaponType;

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
	virtual		HRESULT					Initialize(void* pArg = nullptr) override;
	virtual		void					Update(_float DeltaTime) override;

	const PLAYER_STATE&					GetState() { return m_StateData; }
	_bool								ChangeState(const _wstring& LayerTag, const _wstring& StateTag);

	void								SetAiming(_bool	bFlag) { m_StateData.bIsAiming = bFlag; }
	void								SetWeapon(_uInt	iWeapon) { m_StateData.iWeaponType = iWeapon; }
	void								SetAttack(_bool	bFlag) { m_StateData.bIsAttacking = bFlag; }
	void								SetDireaction(DIRECTION eType) { m_StateData.eDireaction = eType; }

	_string								GetStateFullName();
	_uInt								NextStatePhase(const _wstring& LayerTag);
	_uInt								GetStatePhase(const _wstring& LayerTag);


private : 
	PLAYER_STATE						m_StateData = {};

private :
	HRESULT								ADD_PlayerLayer();
	void								SettingPlayerState(void* pArg = nullptr);

	_bool								StateChildChangeAble(CStateLayer* pLayer, const _wstring& StateTag);

public:
	static		CPlayerStateMachine*		Create();
	virtual		void						Free() override;

};
NS_END