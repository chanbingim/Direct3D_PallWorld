#pragma once
#include "Client_Define.h"
#include "PartObject.h"

NS_BEGIN(Client)
class CItemBase;

class CPlayerItemSlot : public CPartObject
{
public :
	enum class WEAPON_STATE { CHARGE, CHARGE_LOOP, ATTACK, IDLE, END };
	typedef struct ItemSlotDesc : public PARTOBJECT_DESC
	{
		_uInt				iSlotIndex;
	}ITEM_SLOT_DESC;

protected :
	CPlayerItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerItemSlot(const CPlayerItemSlot& rhs);

	virtual ~CPlayerItemSlot() = default;
public :
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;
	
	void									ChangeModelBuffer(CModel* pModel, _bool bIsAnim = false);
	
	void									ChangeWeaponState(WEAPON_STATE eWeaponState, _bool bIsAnimLoop);
	const WEAPON_STATE&						GetWeaponState() { return m_eState; }

protected :
	virtual     HRESULT						Bind_ShaderResources();
	virtual     HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex);

protected :
	const	CItemBase*						m_CurrentEuipItemInfo = {};
	_bool									m_bIsAnimFinished = false;
	_bool									m_bIsAnimWeapon = false;

	CShader*								m_pNoneAimShader = nullptr;
	_uInt									m_iSlotIndex = {};

	WEAPON_STATE							m_eState;

protected :
	void									ChangeAnimWaponAnimationIndex();

private :
	HRESULT									ADD_Components();


public :
	static			CPlayerItemSlot*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END