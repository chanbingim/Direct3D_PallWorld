#pragma once

#include "Client_Define.h"
#include "ContainerObject.h"

#define P_WALK_SPEED 25.f
#define P_JOG_SPEED 50.f
#define P_RUN_SPEED 100.f

NS_BEGIN(Client)
class CPlayerStateMachine;
class CPlayerCamera;

class CPlayer : public CContainerObject
{
protected:
	CPlayer(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

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

private :
	CPlayerCamera*							m_pPlayerCamera = nullptr;
	CPlayerStateMachine*					m_pPlayerFSM = nullptr;

	// 플레이어의 현재 방향
	DIREACTION								m_eDireaction;
	_bool									m_bIsAnimLoop = true;
	_float									m_fMoveSpeed = P_WALK_SPEED;

	_float3									m_PreMovePos;
	_float3									m_PrePlayerRot;

private:
	void									Key_Input(_float fDeletaTime);
	void									MoveAction(_float fDeletaTime);
	void									PlayerMoveView(_float fDeletaTime);

	void									ChangeAction(_float fDeltaTime);

	HRESULT									ADD_PlayerStateMachine();
	HRESULT									ADD_PlayerCamera();
	HRESULT									ADD_PartObejcts();

public:
	static			CPlayer*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END