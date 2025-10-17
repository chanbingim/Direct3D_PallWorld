#pragma once

#include "Client_Define.h"
#include "PellStructData.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CNavigation;
class CCollision;
NS_END

NS_BEGIN(Client)
class CNeturalPellInfo;
class CPellStateMachine;
class CRecovery;
class CPellBody;

class CPellBase : public CContainerObject
{
public :
	enum class PELL_TEAM	{ FRENDLY, NEUTRAL, ENEMY , END };

protected:
	CPellBase(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CPellBase(const CPellBase& rhs);
	virtual ~CPellBase() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

	const PELL_INFO&						GetPellInfo() const { return m_PellInfo; }

protected :
	PELL_TEAM								m_eTeam;

	_bool									m_bIsAction = false;
	_bool									m_bIsLoop = true;

	_float									m_fAccActionTime = 0;
	_float									m_fActionTime = 0;
	CPellStateMachine*						m_pPellFsm = nullptr;

#pragma region Component

#pragma region Collision
	CCollision*								m_pCollision = nullptr;
#pragma endregion

#pragma region Recovery
	_float									m_RecoverTime = 0.7f;
	_float									m_RecoverSetemina = 10.f;
	CRecovery*								m_pRecovery = nullptr;
#pragma endregion

#pragma region Navigation
	CNavigation*							m_pNevigation = nullptr;
#pragma endregion

#pragma endregion

#pragma region Part Object
	CPellBody*								m_pPellBody = nullptr;

#pragma region HEALTBAR
	_float									m_fInfoVisibleDistance = 5.f;
	CNeturalPellInfo*						m_pNeturalPellUI = nullptr;
#pragma endregion

#pragma endregion

	PELL_INFO								m_PellInfo = {};
	list<_float3>							m_PathFinding;

	_float3									m_vTargetPoint = { -1.f, -1.f, -1.f};
	_float									m_LerpTime = {};

protected :
	HRESULT									ADD_PellInfoUI();

	void									PellPlayFSM(_float fDeletaTime);
	void									PellChiceAction();
	void									PellTackingAction();

	void									ActionFrendly();
	void									ActionNeutral();
	void									ActionEnemy();

	void									ShowPellInfo();


public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END