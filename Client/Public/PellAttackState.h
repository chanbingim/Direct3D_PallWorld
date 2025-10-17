#pragma once
#include "Client_Define.h"
#include "PellSkillStruct.h"
#include "State.h"

NS_BEGIN(Client)
class CPellBase;

class CPellAttackState : public CState
{
public :
	typedef struct PellAttacStatekDesc
	{
		const CPellBase*			ActPell;
		PELL_SKILL_DATA*			AttackData;

		_float*						fSkillMoveSpeed;
		_bool						IsSpaceOut;
	}PELL_ATTACK_STATE_DESC;

private:
	CPellAttackState(const char* szStateName);
	virtual ~CPellAttackState() = default;

public:
	virtual void OnStateEnter(void* pArg = nullptr) override;
	virtual void OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void OnStateExit(void* pArg = nullptr) override;

private :
	const CPellBase*				m_pActPell = nullptr;
	_float*							m_fSkillMovePtr = nullptr;

	_bool							m_IsSpaceOut;
	PELL_SKILL_DATA					m_AttackData = {};

	_float							m_fAccTime = 0.0f;
	_float							m_DurationTime = {};

private :
	void							SkillMotionChange(_float fTimeDeleta);

public:
	static CPellAttackState*	Create(const char* szStateName);
	virtual void				Free() override;
};
NS_END