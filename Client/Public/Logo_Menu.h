#pragma once

#include "BackGround.h"

NS_BEGIN(Client)
class CLogo_Menu final : public CBackGround
{
private:
	CLogo_Menu(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CLogo_Menu(const CLogo_Menu& rhs);
	virtual ~CLogo_Menu() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private:
	HRESULT									ADD_Childs();
	HRESULT									ADD_Components();

public:
	static			CLogo_Menu*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END