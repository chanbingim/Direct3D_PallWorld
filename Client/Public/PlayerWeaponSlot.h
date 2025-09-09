#pragma once
#include "Client_Define.h"
#include "PartObject.h"

NS_BEGIN(Client)
class CPlayerWeaponSlot : public CPartObject
{
public :
	enum class WEAPON_STATE { IDLE, CHARGE, CHARGE_LOOP, ATTACK, END };
	typedef struct WeaponSlotDesc : public PARTOBJECT_DESC
	{
		_uInt				iSlotIndex;
		const _float4x4*	pLeftSocketMatrix;
	}WEAPON_SLOT_DESC;

protected :
	CPlayerWeaponSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerWeaponSlot(const CPlayerWeaponSlot& rhs);

	virtual ~CPlayerWeaponSlot() = default;
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
	const WEAPON_STATE&						GetWeaponState() { return m_eState; }

protected :
	virtual     HRESULT						Bind_ShaderResources();
	virtual     HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex);

private :
	_bool									m_bIsAnimWeapon = false;
	_bool									m_LeftRightFlag = false;
	const _float4x4*						m_pLeftSocket = nullptr;

	CShader*								m_pNoneAimShader = nullptr;
	_uInt									m_iSlotIndex = {};
	WEAPON_STATE							m_eState;

private :
	HRESULT									ADD_Components();

public :
	static			CPlayerWeaponSlot*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END