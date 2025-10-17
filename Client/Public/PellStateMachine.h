#pragma once
#include "Client_Define.h"
#include "FiniteStateMachine.h"

NS_BEGIN(Engine)
class CContainerObject;
NS_END

NS_BEGIN(Client)

class CPellStateMachine : public CFiniteStateMachine
{
public:
	enum class MOVE_ACTION { DEFAULT, PATROL, RESET, WORK, CARRY, LAUNCHED, END };
	//전투 상태 및 비 전투 상태에 대한 Layer 구조
	enum class COMBAT_ACTION { ATTACK,  HIT, STUN, DEAD, END };

	typedef		struct  Pell_State 
	{
		_bool				bIsAiming;
		_bool				bIsCombat;
		_bool				bIsAttacking;
		DIRECTION			eDireaction;
		_uInt				iWeaponType;

		MOVE_ACTION			eMove_State;
		COMBAT_ACTION		eCombat_State;
	}PELL_STATE;


protected:
	CPellStateMachine();
	CPellStateMachine(const CPellStateMachine& rhs);
	virtual ~CPellStateMachine() = default;

public:
	virtual		HRESULT					Initialize(void* pArg = nullptr) override;
	virtual		void					Update(_float DeltaTime, void* pArg = nullptr) override;


	_bool								ChangeState(const _wstring& LayerTag, const _wstring& StateTag, void* pArg = nullptr);
	void								PellStateReset(const _wstring& LayerTag);

	void								SetAiming(_bool	bFlag) { m_StateData.bIsAiming = bFlag; }
	void								SetWeapon(_uInt	iWeapon) { m_StateData.iWeaponType = iWeapon; }
	void								SetAttack(_bool	bFlag) { m_StateData.bIsAttacking = bFlag; }
	void								SetDireaction(DIRECTION eType) { m_StateData.eDireaction = eType; }
	void								SetCombatAction(_bool bFlag) { m_StateData.bIsCombat = bFlag; }

	const PELL_STATE&					GetState() { return m_StateData; }
	_bool								GetLayerLastPhase(const _wstring& LayerTag);

	_string								GetAnimationName();

	void								CombatStateReset();

private:
	PELL_STATE							m_StateData = {};
	_bool								m_AnimationLoop = true;

private :
	HRESULT								ADD_StateLayer();
	HRESULT								SetUp_PellInitState(void* pArg = nullptr);

public:
	static		CPellStateMachine*			Create();
	virtual		void						Free() override;

};
NS_END