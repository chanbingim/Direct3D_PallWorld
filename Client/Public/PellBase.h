#pragma once

#include "Client_Define.h"
#include "PellStructData.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CNavigation;
class CCollision;
class CChaseComponent;
NS_END

NS_BEGIN(Client)
class CCombatComponent;
class CNeturalPellInfo;
class CTerrainManager;
class CPellStateMachine;
class CRecovery;
class CPellBody;

class CPellBase : public CContainerObject
{
public :
	typedef struct PellBaseDesc : public GAMEOBJECT_DESC
	{
		_bool						bIsPellData;
		PELL_INFO					PellInfo;
	}PELL_BASE_DESC;

public :
	enum class PELL_TEAM	{ FRENDLY, NEUTRAL, ENEMY, END };

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
	const _bool								GetFinisehdAnimation() const;

	virtual		void						Damage(void* pArg, CActor* pDamagedActor);

	void									ChangePellTeam(PELL_TEAM eTeam);
	PELL_TEAM								GetPellTeam() { return m_eTeam; }

	void									ChangePellStorageType(PELL_STORAGE_STATE eStorageType);

#pragma region Work State
	void									ChangePellWork(CGameObject*	pWorkObject);
	_bool									bIsWorkAble() const;
	void									ResetWorkSate();
#pragma endregion

#pragma region Pal Carry
	void									ChangePellCarry(const _float4x4* pSocketMatrix);
	void									ResetCarryState();
	void									PellLaunched(_float3 vDir, _float ThorwSpeed);
#pragma endregion
	
	void									AttachSocket(const _float4x4* pSocket, const _char SocketFlag) const;
	const CPellBody*						GetPellBody() const { return m_pPellBody; }

#pragma region Friendly Team Function
	void									SpawnPellFriendly();
#pragma endregion

protected :
	_uInt									m_PellID;
	PELL_TEAM								m_eTeam;
	PELL_INFO								m_PellInfo = {};

	_float									m_fPellMoveSpeed = {};
	_bool									m_bIsLoop = true;
	_bool									m_bIsAction = false;
	
	_float									m_fAccActionTime = 0;

	CGameObject*							m_pTargetObject = nullptr;
	CPellStateMachine*						m_pPellFsm = nullptr;
	CChaseComponent*						m_pChase = nullptr;
	CTerrainManager*						m_pTerrainManager = nullptr;

#pragma region Component
#pragma region Navigation
	const WCHAR*							m_pChunkName = {};
	CTerrainManager*						m_pTerrainManager = nullptr;
	CNavigation*							m_pNevigation = nullptr;
#pragma endregion

#pragma region Collision
	CCollision*								m_pCollision = nullptr;
	CPellBody*								m_pPellBody = nullptr;
	CCombatComponent*						m_pCombatCom = nullptr;
#pragma endregion

#pragma region Recovery
	_float									m_RecoverTime = 0.7f;
	_float									m_RecoverSetemina = 10.f;
	CRecovery*								m_pRecovery = nullptr;
#pragma endregion

	_float									m_fInfoVisibleDistance = 5.f;
	CNeturalPellInfo*						m_pNeturalPellUI = nullptr;
#pragma endregion
	
	list<_float3>							m_PathFinding;

	_float3									m_vTargetPoint = { -1.f, -1.f, -1.f};

protected :
	HRESULT									SetUpDefaultPellData(_bool bIsFlag, const PELL_INFO& Pellinfo);
	HRESULT									ADD_PellInfoUI();
	// 전투를 위한 전투 기능
	virtual		void						CombatAction(_float fDeletaTime, CGameObject* pTarget);
	virtual		void						OverlapEvent(_float3 vDir, CGameObject* pHitObject);

	_bool									PellPlayFSM(_float fDeletaTime);
	void									StartMoveAction(const _float3 vEndPos);
	void									SettingNavigation();


private :
	// 이거 펠 파츠오브젝트 세팅부터 하고 불러야함 매번 코드짜서 넣어주기 귀찮아서
	// 공통으로 묶어둔거라 파츠오브젝트 세팅을 안하고 부를경우 파츠오브젝트 크기를 못받아서 터짐
	void									PellChiceAction(_float fDeletaTime);
	void									UpdateTeamAction(_float fDeletaTime);

	void									PellTackingAction(_float fDeletaTime);
#pragma region Frendly
	// 아군일떄
	void									ActionFrendly(_float fDeletaTime);
	
	void									UpdateFrendlyAction(_float fDeletaTime);
#pragma endregion
	
#pragma region Netural
	// 야생 몬스터 일떄
	void									ActionNeutral(_float fDeletaTime);

	// 야생펠 죽음	
	void									DeadNeutalPell();
#pragma endregion

	// NPC가 가지고있는 몬스터일때 
	void									ActionEnemy(_float fDeletaTime);

	// HP Bar를 보여준다.
	void									ShowPellInfo();

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END