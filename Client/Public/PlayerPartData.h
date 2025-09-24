#pragma once
#include "Client_Define.h"
#include "PartObject.h"

NS_BEGIN(Client)

class CPlayerBody;
class CPlayerItemSlot;

class CPlayerPartData : public CPartObject
{
private:
	CPlayerPartData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerPartData(const CPlayerPartData& rhs);
	virtual ~CPlayerPartData() = default;

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
	_bool									IsAnimFinished() { return m_bIsFinished; }

	void									SetUppderAnimation(_uInt iIndex, _bool bIsSplite);
	void									ChangeSocketFlag(_char bitFlag);

	void									ChangeWeaponState(_uInt iWeaponState, _bool bIsAnimLoop = true);
	void									NearAttackOnCollision();
	void									RoatationPitchSpine(_float fPitchAngle);


private:
	CPlayerBody*							m_pPlayerBody = nullptr;

	//무기 소켓 위치 및 손위치
	CPlayerItemSlot*						m_pWeaponSocket[3] = {};
	const _float4x4*						m_pWeaponSocketMatrix[3];
	_float4x4*								m_pSpineOffsetMatrix = nullptr;


	_bool									m_bIsFinished = false;
	_bool									m_SplitAnimation = false;
	_uInt									m_UpperBodyIndex = {};

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_ChildObject();
	HRESULT									ADD_AnimParts();
	HRESULT									Insert_AnimKeyFrameFunction();

	HRESULT									ShootProjecttileObject();


public:
	static			CPlayerPartData*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END