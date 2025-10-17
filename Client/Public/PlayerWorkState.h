#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Client)
class CArchitecture;
class CPlayerPartData;

class CPlayerWorkState : public CState
{
public :
	enum class WORK_TYPE { BUILD, CREATE, END};

	typedef struct PlayerWorkState
	{
		WORK_TYPE				eWorkType;

		const CArchitecture*	pArchitecture;
		const CPlayerPartData*  pPlayerAnim;

		_bool*					bIsAnimLoop;
	}PLAYER_WORK_STATE;

protected:
	CPlayerWorkState(const char* szStateName);
	virtual ~CPlayerWorkState() = default;

public:
	virtual void				OnStateEnter(void* pArg = nullptr)		override;
	virtual void				OnStateExcution(_float fDeletaTime, void* pArg = nullptr)	override;
	virtual void				OnStateExit(void* pArg = nullptr)		override;

private:
	const CArchitecture*		m_pArchitecture = nullptr;
	const CPlayerPartData*		m_pPlayerAnim = nullptr;

	WORK_TYPE					m_eWorkType = {};
	_bool*						m_bIsAnimLoop = nullptr;

private :
	void						NextPhaseMove();

public:
	static CPlayerWorkState*	Create(const char* szStateName);
	virtual void				Free() override;

};
NS_END