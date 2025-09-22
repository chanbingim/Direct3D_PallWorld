#pragma once

#include "Client_Define.h"
#include "FiniteStateMachine.h"

NS_BEGIN(Client)
class CPlayerStateMachine final : public CFiniteStateMachine
{
public:
	// 상체 및 하체 이동에 대한 상태 구현
	enum class MOVE_ACTION { DEFAULT,  CROUCH, CLIMB, JUMP, END };
	enum class MOVE_CHILD_ACTION { WALK, IDLE, JOG, SPRINT, END };

	//전투 상태 및 비 전투 상태에 대한 Layer 구조
	enum class COMBAT_ACTION { ATTACK, HIT, DEAD, END };
	enum class NONE_COBAT_ACTION { PETTING, CARRY, SLEEP, COOK, END };

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

		// 전투에서 가져갈 행동 상태
		COMBAT_ACTION		eCombat_State;

		// 비전투에서 움직임을 못 가져가는 녀석들
		NONE_COBAT_ACTION	eNone_Combat_State;
	}PLAYER_STATE;

protected:
	CPlayerStateMachine();
	CPlayerStateMachine(const CPlayerStateMachine& rhs);
	virtual ~CPlayerStateMachine() = default;

public:
	virtual		HRESULT					Initialize(void* pArg = nullptr) override;
	virtual		void					Update(_float DeltaTime, void* pArg = nullptr) override;

	const PLAYER_STATE&					GetState() const { return m_StateData; }
	_bool								ChangeState(const _wstring& LayerTag, const _wstring& StateTag, void* pArg = nullptr);

	void								SetAiming(_bool	bFlag) { m_StateData.bIsAiming = bFlag; }
	void								SetWeapon(_uInt	iWeapon) { m_StateData.iWeaponType = iWeapon; }
	void								SetAttack(_bool	bFlag) { m_StateData.bIsAttacking = bFlag; }
	void								SetDireaction(DIRECTION eType) { m_StateData.eDireaction = eType; }

	_string								GetStateFullName();
	_string								GetLayerAimStateName();

	void								PlayerStateReset(const _wstring& LayerTag);

private : 
	PLAYER_STATE						m_StateData = {};

private :
	HRESULT								ADD_PlayerLayer();
	void								SettingPlayerState(void* pArg = nullptr);

	_bool								StateChildChangeAble(CStateLayer* pLayer, const _wstring& StateTag);
	const char*							GetWeaponName(_uInt eWeaponType);

public:
	static		CPlayerStateMachine*		Create();
	virtual		void						Free() override;

};
NS_END