#pragma once
#include "Client_Define.h"
#include "ProgressBar.h"

NS_BEGIN(Client)
class CPellHealthBar final : public CProgressBar
{
protected:
	CPellHealthBar(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CPellHealthBar(const CPellHealthBar& rhs);
	virtual ~CPellHealthBar() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private:
	HRESULT									ADD_Components();


public:
	static			CPellHealthBar*			Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END