#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CButtonFlare : public CBackGround
{
private:
	CButtonFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButtonFlare(const CButtonFlare& rhs);
	virtual ~CButtonFlare() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

protected :
	HRESULT									Apply_ConstantShaderResources();

private :
	static		_float4						m_vColor;

private:
	HRESULT									ADD_Components();

public:
	static			CButtonFlare*			Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END