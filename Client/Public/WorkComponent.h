#pragma once
#include "Client_Define.h"
#include "Component.h"

NS_BEGIN(Engine)
class CNavigation;
NS_END

NS_BEGIN(Client)
class CPellBase;
class CWorkAbleObject;

class CWorkComponent : public CComponent
{
public :
	typedef struct WorkComDesc
	{
		const CPellBase*				pOwner;
	}WORK_COMPONENT_DESC;
private :
	CWorkComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorkComponent(const CWorkComponent& Prototype);
	virtual ~CWorkComponent() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual	HRESULT					Render() override;
	void							Update(_float fDeletaTime);

public :
	// 맨처음에 여기서 타겟 오브젝트까지의 최적의 경로를 구한다.
	void							SetWorkTargetObject(CWorkAbleObject* pWorkTarget, CNavigation* pNavigation);
	
	void							NextWorkPhase();

	// 현재 어떤 오브젝트가 타겟으로 선택되어있는지를 반환
	const	CWorkAbleObject*		GetWorkTarget();
	_bool							IsWorkFinished() { return m_bIsFinished; }
	_bool							IsGotoWork() { return m_bIsGoToWork; }
	_bool							IsWorking() { return m_bIsWorking; }

private :
	const CPellBase*				m_pOwner = nullptr;
	CWorkAbleObject*				m_pWorkTarget = nullptr;
	CNavigation*					m_pNavigation = nullptr;
	
	list<_float3>					m_pPathList = {};
	_float							m_fWorkMoveSpeed = {};
	_float3							m_vTargetPoint = {};

	_bool							m_bIsGoToWork = false;
	_bool							m_bIsFinished = false;
	_bool							m_bIsWorking = false;
	_bool							m_SettingData = false;

private :
	void							WorkPathFinding();
	void							GotToWork(_float fDeletaTime);
	void							Working(_float fDeletaTime);

#pragma region WORK FUNC
	void							CommoneWorkFunc(_float fDeletaTime);
	void							MinningWorkFunc(_float fDeletaTime);
	void							TransportWorkFunc(_float fDeletaTime);
#pragma endregion


public:
	static		CWorkComponent*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CWorkComponent*		Clone(void* pArg) override;
	virtual		void				Free() override;

};
NS_END