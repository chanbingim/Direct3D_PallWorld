#pragma once
#include "Client_Define.h"
#include "Slider.h"

NS_BEGIN(Client)
class CInvenSlider : public CSlider
{
protected:
	CInvenSlider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInvenSlider(const CInvenSlider& rhs);
	virtual ~CInvenSlider() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	_float									GetCurPercent() { return m_fSliderCurPercent; }

protected:
	virtual     HRESULT						Bind_ShaderResources() override;
	virtual     HRESULT						Apply_ConstantShaderResources() override;

	virtual		void						MouseButtonDwon() override;
	virtual		void						MouseButtonPressed() override;
	virtual		void						MouseButtonUp() override;

private :
	_float4									m_vColor = {};
	ID3DX11EffectVectorVariable*			m_pShader_Color = nullptr;

private :
	HRESULT									ADD_Components();
	HRESULT									CreateSliderButton();


public:
	static			CInvenSlider*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CUserInterface*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END