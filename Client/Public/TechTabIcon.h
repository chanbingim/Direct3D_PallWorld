#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CTechTabIcon : public CBackGround
{
private:
	CTechTabIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechTabIcon(const CTechTabIcon& rhs);
	virtual ~CTechTabIcon() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;
	void									SetTexIcon(_uInt iTexIconIndex) { m_iTechTexIndex = iTexIconIndex; }

private:
	//제작 UI
	_uInt									m_iTechTexIndex = {};

private:
	HRESULT									ADD_Components();

public:
	static		CTechTabIcon*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END