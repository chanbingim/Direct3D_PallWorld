#pragma once

#include "Button.h"

NS_BEGIN(Client)
class COptionButton : public CButton
{
private:
	COptionButton(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	COptionButton(const COptionButton& rhs);
	virtual ~COptionButton() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									Bind_ClickEvent(function<void()> Func);

private:
	HRESULT									ADD_Components();

public:
	static		COptionButton*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;
};
NS_END