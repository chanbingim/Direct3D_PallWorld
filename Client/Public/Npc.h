#pragma once
#include "Client_Define.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CNavigation;
class CCollision;
NS_END

NS_BEGIN(Client)
class CNpcStateMachine;
class CTerrainManager;
class CNpcBody;

class CNpc : public CContainerObject
{
public:
	enum class NPC_TEAM { FRENDLY, NEUTRAL, ENEMY, END };

protected:
	CNpc(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CNpc(const CNpc& rhs);
	virtual ~CNpc() = default;

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
	const _bool								GetFinisehdAnimation() const;

	NPC_TEAM								GetNpcTeam() { return m_eTeam; }

protected:
	_uInt									m_NpcID;
	NPC_TEAM								m_eTeam;

	_float									m_fActionDistance = {};

	_bool									m_bIsLoop = true;
	_bool									m_bIsAction = false;

	_wstring								m_szChunkName = {};
	CTerrainManager*						m_pTerrainManager = nullptr;
	CNavigation*							m_pNevigation = nullptr;

	CNpcStateMachine*						m_pNpcFsm = nullptr;

#pragma region Component

#pragma region Collision
	CCollision*								m_pCollision = nullptr;
	CNpcBody*								m_pNpcBody = nullptr;
#pragma endregion

protected:
	HRESULT									SetUpDefaultNpcData();
	HRESULT									ADD_PellInfoUI();

	_bool									NpcPlayFSM(_float fDeletaTime);
	void									SettingNavigation();

private :
	// 상호작용이 가능한지
	void									ShowActionUI();

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END