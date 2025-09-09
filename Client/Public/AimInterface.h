#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CAimInterface : public CBackGround
{
private :
	CAimInterface(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAimInterface(const CAimInterface& rhs);
	virtual ~CAimInterface() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private :
	HRESULT									ADD_Components();
	_bool									m_IsAiming = false;


public:
	static		CAimInterface*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END