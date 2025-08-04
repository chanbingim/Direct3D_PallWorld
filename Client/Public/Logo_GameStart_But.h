#pragma once

#include "BackGround.h"

NS_BEGIN(Client)
class CLogo_GameStart_But final : public CBackGround
{
protected:
	CLogo_GameStart_But(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CLogo_GameStart_But(const CLogo_GameStart_But& rhs);
	virtual ~CLogo_GameStart_But() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

protected:
	virtual		void						MouseHoverEnter();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();

private :
	HRESULT									ADD_Components();

public:
	static		CLogo_GameStart_But*		Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END