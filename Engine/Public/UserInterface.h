#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)
class ENGINE_DLL CUserInterface abstract : public CGameObject
{
protected:
	CUserInterface(ID3D11Device * pGraphic_Device, ID3D11DeviceContext * pDeviceContext);
	CUserInterface(const CUserInterface& rhs);
	virtual ~CUserInterface() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	//Function
	void									SetZOrder(_uInt Number) { m_iZOrder = Number; }
	_uInt									GetZOrder();

	void									SetLocation(_float3 vPosition);
	void									SetRotation(_float3 vRotation);
	void									SetScale(_float3 vScale);

	void									ADDPosition(_vector vAddPos);
	void									ADDRotation(_vector vAxis, _float fTurnSpeed, _float fTimeDeleta);

	_float2									GetViewPos();
	const		RECT&						GetRectSize();
protected :
	_float4x4								m_ViewMatrix = {};
	_float4x4								m_ProjMatrix = {};

	RECT									m_UISize = {};
	_float2									m_fPos = {};

	_bool									m_bIsHover = false;
	_bool									m_bIsMouseEvent = false;

	_uInt									m_iZOrder = {};
	_bool									m_bIsWorld = false;

protected :
	virtual			void					MouseHoverEnter();
	virtual			void					MouseHovering();
	virtual			void					MouseHoverExit();

	virtual			void					MouseButtonDwon();
	virtual			void					MouseButtonPressed();
	virtual			void					MouseButtonUp();

	virtual     HRESULT						Bind_ShaderResources() override;
	virtual     HRESULT						Apply_ConstantShaderResources() override;

	_bool									IsHover() { return m_bIsHover; }

private :
	void									OverlapEvent();
	void									UpdateRectSize();

public:
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END