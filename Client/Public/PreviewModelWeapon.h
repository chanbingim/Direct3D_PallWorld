#pragma once

#include "PlayerItemSlot.h"

NS_BEGIN(Engine)
class CTexture;
NS_END

NS_BEGIN(Client)
class CPreviewModelWeapon : public CPlayerItemSlot
{
public:
	typedef struct WeaponSlotDesc : public ITEM_SLOT_DESC
	{
		const _float4x4* pLeftSocket = nullptr;
	}WEAPON_SLOT_DESC;

protected:
	CPreviewModelWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPreviewModelWeapon(const CPreviewModelWeapon& rhs);
	virtual ~CPreviewModelWeapon() = default;
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

private:
	_bool									m_LeftFlag = false;
	const _float4x4*						m_pLeftSocket = nullptr;

public:
	static			CPreviewModelWeapon*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END