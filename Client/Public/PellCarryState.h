#pragma once
#include "Client_Define.h"
#include "State.h"

NS_BEGIN(Engine)
class CTransform;
class CGameInstance;
class CCollision;
NS_END

NS_BEGIN(Client)
class CPellBase;

class CPellCarryState : public CState
{
public :
	typedef struct PellCarryDesc
	{
		const CPellBase*			pActPell;
		const _float4x4*			pSoketMat;

		CCollision*					pPellCollision;
	}PELL_CARRY_DESC;

private:
	CPellCarryState(const char* szStateName);
	virtual ~CPellCarryState() = default;

public:
	virtual void				OnStateEnter(void* pArg = nullptr) override;
	virtual void				OnStateExcution(_float fDeletaTime, void* pArg = nullptr) override;
	virtual void				OnStateExit(void* pArg = nullptr) override;

private :
	CGameInstance*				m_pGameInstance = nullptr;
	CCollision*					m_pPellCollision = nullptr;

	const	CPellBase*			m_pActPell = nullptr;
	const	_float4x4*			m_pSoketMat = nullptr;
	_float4x4					m_CombinedMat = {};

private :
	void						UpdateStateState();
	void						NextPhaseAnimation();

public:
	static CPellCarryState*		Create(const char* szStateName);
	virtual void				Free() override;

};
NS_END