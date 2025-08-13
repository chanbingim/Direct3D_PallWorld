#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CSwapChain
{



private :
	IDXGISwapChain*				m_pSwapChain = nullptr;
	ID3D11RenderTargetView*		m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;
	D3D11_VIEWPORT				m_ViewPort = {};

public:
	static		CSwapChain*		Create(const ENGINE_DESC& Engine_Desc);
	virtual		void			Free() override;
};
NS_END