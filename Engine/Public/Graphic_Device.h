#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CGraphic_Device : public CBase
{
private:
	CGraphic_Device() = default;
	virtual ~CGraphic_Device() = default;

public :
	HRESULT		Initialize_Grphic(const ENGINE_DESC& Engine_Desc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	void		Render_Begin(_float* Color);
	void		Render_End();

private :
	ID3D11Device*				m_pGraphicDevice = nullptr;
	IDXGISwapChain*				m_pSwapChain = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;
	ID3D11RenderTargetView*		m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;
	D3D11_VIEWPORT				m_ViewPort = {};

	_float						m_ClearColor[4] = {0.f, 0.f, 1.f, 1.f};

private :
	HRESULT						Initialize_SwapChain(const ENGINE_DESC& Engine_Desc);
	HRESULT						Initialize_RenderTarget();
	HRESULT						Initialize_DepthStencil(_uInt WinSizeX, _uInt WinSizeY);

public :
	static		CGraphic_Device*		Create(const ENGINE_DESC& Engine_Desc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	virtual		void					Free() override;
};
NS_END