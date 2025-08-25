#pragma once
#include "Base.h"

NS_BEGIN(Engine)

typedef struct WindowRender_Desc
{
	IDXGISwapChain*				pSwapChain = nullptr;
	ID3D11RenderTargetView*		pRenderTargetView = nullptr;
	ID3D11DepthStencilView*		pDepthStencilView = nullptr;
	D3D11_VIEWPORT				ViewPort = {};

}WIN_RENDER_DESC;

class CGraphic_Device : public CBase
{
private:
	CGraphic_Device() = default;
	virtual ~CGraphic_Device() = default;

public :
	HRESULT				Initialize_Grphic(const ENGINE_DESC& Engine_Desc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	void				Render_Begin(_float* Color);
	void				Render_End();

	HRESULT				ADD_Window(const ENGINE_DESC& Win_Desc);
	void				Set_RenderResource(_uInt iIndex);

	HRESULT				GetBackBuffer(ID3D11Texture2D** pOut);
	_uInt				GetRenderTargetNum() { return m_CurViewWinow; }

private :
	ID3D11Device*				m_pGraphicDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;
	vector<WIN_RENDER_DESC>		m_Render_Dsec;

	_uInt						m_CurViewWinow = {};
	_float						m_ClearColor[4] = {0.f, 0.f, 1.f, 1.f};

private :
	HRESULT						Initialize_SwapChain(const ENGINE_DESC& Engine_Desc, IDXGISwapChain** pOut);
	HRESULT						Initialize_RenderTarget(IDXGISwapChain* pSwapChan, ID3D11RenderTargetView** ppOut);
	HRESULT						Initialize_DepthStencil(_uInt WinSizeX, _uInt WinSizeY, ID3D11DepthStencilView** ppOut);

public :
	static		CGraphic_Device*		Create(const ENGINE_DESC& Engine_Desc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	virtual		void					Free() override;
};
NS_END