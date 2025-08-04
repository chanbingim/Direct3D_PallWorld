#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CPellLogo :  public CBackGround
{
private:
	CPellLogo(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CPellLogo(const CPellLogo& rhs);
	virtual ~CPellLogo() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private:
	HRESULT									ADD_Components();

public:
	static			CPellLogo*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END