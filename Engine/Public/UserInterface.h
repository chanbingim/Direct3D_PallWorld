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
	// 랜더
	virtual		HRESULT						Render() override;

	//Function
	void									SetZOrder(_uInt Number) { m_iZOrder = Number; }
	_uInt									GetZOrder();

protected :
	RECT									m_UISize = {};
	_bool									m_bIsHover = false;
	_uInt									m_iZOrder = {};

protected :
	_bool									IsHover() { return m_bIsHover; }

private :
	void									OverlapEvent();
	void									UpdateRectSize();

public:
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END