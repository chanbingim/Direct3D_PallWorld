#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CWeaponSlotIcon : public CBackGround
{
protected:
	CWeaponSlotIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeaponSlotIcon(const CWeaponSlotIcon& rhs);
	virtual ~CWeaponSlotIcon() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetIconTexture(const CTexture* pIconTexture);

private :
	const CTexture*							m_pItemIcon = nullptr;

private :
	HRESULT									ADD_Components();

public:
	static			CWeaponSlotIcon*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END