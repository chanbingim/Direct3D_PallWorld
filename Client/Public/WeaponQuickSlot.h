#pragma once
#include "BackGround.h"

NS_BEGIN(Client)

class CWeaponSlotIcon;
class CWeaponQuickSlotName;
class CWeaponQuickSlotCount;

class CWeaponQuickSlot : public CBackGround
{
protected:
	CWeaponQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeaponQuickSlot(const CWeaponQuickSlot& rhs);
	virtual ~CWeaponQuickSlot() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private :
	CWeaponSlotIcon*						m_pWeaponSlotIcon = nullptr;
	CWeaponQuickSlotName*					m_pWeaponSlotName = nullptr;
	CWeaponQuickSlotCount*					m_pWeaponSlotCount = nullptr;

	_float4									m_vImageColor;

private :
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static			CWeaponQuickSlot*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END