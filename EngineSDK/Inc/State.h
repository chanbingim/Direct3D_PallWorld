#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CState abstract : public CBase
{
public :
	enum class ACTION_PHASE { START, STARTLOOP, ENDLOOP, END };

protected :
	CState(const char* szStateName);
	virtual ~CState() = default;

public :
	virtual void OnStateEnter(void* pArg = nullptr) = 0;
	virtual void OnStateExcution(_float fDeletaTime, void* pArg = nullptr) = 0;
	virtual void OnStateExit(void* pArg = nullptr) = 0;

	_uInt					NexPhase();
	_uInt					GetPhaseIndex()		{ return m_iPhaseIndex; }
	const char*				GetStateName()		{ return m_szStateName.c_str(); }
	const _bool				GetStateAnimLoop()	{ return m_bStateAnimLoop; }

protected :
	_string					m_szStateName;

	_bool					m_bStateAnimLoop = true;
	_uInt					m_iPhaseIndex = {};

public :
	virtual void	Free() override;
};
NS_END