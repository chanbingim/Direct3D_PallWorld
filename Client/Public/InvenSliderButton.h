#pragma once
#include "Client_Define.h"
#include "SliderButton.h"

NS_BEGIN(Client)
class CInvenSliderButton : public CSliderButton
{
protected:
	CInvenSliderButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInvenSliderButton(const CInvenSliderButton& rhs);
	virtual ~CInvenSliderButton() = default;

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
	virtual     HRESULT						Bind_ShaderResources() override;
	virtual     HRESULT						Apply_ConstantShaderResources() override;

private:
	_float4									m_vColor = {};
	ID3DX11EffectVectorVariable*			m_pShader_Color = nullptr;

private :
	HRESULT									ADD_Components();

public:
	static			CInvenSliderButton*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CUserInterface*			Clone(void* pArg);
	virtual			void					Free() override;
	 
};
NS_END