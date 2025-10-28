#pragma once
#include "Client_Define.h"
#include "PartObject.h"

NS_BEGIN(Client)

class CPlayerBody;
class CPreviewModelWeapon;

class CPreivewModelBody : public CPartObject
{
private:
	CPreivewModelBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPreivewModelBody(const CPreivewModelBody& rhs);
	virtual ~CPreivewModelBody() = default;

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

	void									SetUppderAnimation(_uInt iIndex, _bool bIsSplite);
	void									ChangeSocketFlag(_char bitFlag);

	void									ChangeWeaponState(_uInt iWeaponState, _bool bIsAnimLoop = true);
	const _float4x4*						GetLeftHandSocket();

private:
	CPlayerBody*							m_pPlayerBody = nullptr;

	//무기 소켓 위치 및 손위치
	CPreviewModelWeapon*					m_pWeaponSocket = {};
	const _float4x4*						m_pWeaponSocketMatrix;

	const _float4x4*						m_pLeftHandSocket = nullptr;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_ChildObject();

public:
	static			CPreivewModelBody*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END