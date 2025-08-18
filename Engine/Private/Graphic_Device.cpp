#include "Graphic_Device.h"

HRESULT CGraphic_Device::Initialize_Grphic(const ENGINE_DESC& Engine_Desc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
    _uInt		iFlag = 0;

#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL			FeatureLV;

    //DX9에서 하드웨어 설정잡고
    //버전설정하고 Device만드는것과 비슷한 느낌임
    //대신 DX11에서는 Device를 먼저 생성한뒤에 나머지 세팅을 이어간다.
    if(FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pGraphicDevice, &FeatureLV, &m_pDeviceContext)))
        return E_FAIL;

    if (FAILED(ADD_Window(Engine_Desc)))
        return E_FAIL;

    Set_RenderResource(0);
   
    *ppDeviceOut = m_pGraphicDevice;
    *ppContextOut = m_pDeviceContext;

    Safe_AddRef(m_pGraphicDevice);
    Safe_AddRef(m_pDeviceContext);

    return S_OK;
}

void CGraphic_Device::Render_Begin(_float* Color)
{
    if (nullptr == m_pDeviceContext)
        return;

    if(Color)
        m_pDeviceContext->ClearRenderTargetView(m_Render_Dsec[m_CurViewWinow].pRenderTargetView, Color);
    else
        m_pDeviceContext->ClearRenderTargetView(m_Render_Dsec[m_CurViewWinow].pRenderTargetView, m_ClearColor);

    m_pDeviceContext->ClearDepthStencilView(m_Render_Dsec[m_CurViewWinow].pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CGraphic_Device::Render_End()
{
    if (nullptr == m_Render_Dsec[m_CurViewWinow].pSwapChain)
        return;

    m_Render_Dsec[m_CurViewWinow].pSwapChain->Present(0, 0);
}

HRESULT CGraphic_Device::ADD_Window(const ENGINE_DESC& Win_Desc)
{
    WIN_RENDER_DESC Render_Desc;

    if (FAILED(Initialize_SwapChain(Win_Desc, &(Render_Desc.pSwapChain))))
        return E_FAIL;

    if (FAILED(Initialize_RenderTarget(Render_Desc.pSwapChain, &(Render_Desc.pRenderTargetView))))
        return E_FAIL;

    if (FAILED(Initialize_DepthStencil(Win_Desc.iWinSizeX, Win_Desc.iWinSizeY, &(Render_Desc.pDepthStencilView))))
        return E_FAIL;

    //ViewPort Setting
    Render_Desc.ViewPort.TopLeftX = 0.f;
    Render_Desc.ViewPort.TopLeftY = 0.f;
    Render_Desc.ViewPort.Width = (FLOAT)Win_Desc.iWinSizeX;
    Render_Desc.ViewPort.Height = (FLOAT)Win_Desc.iWinSizeY;
    Render_Desc.ViewPort.MinDepth = 0.f;
    Render_Desc.ViewPort.MaxDepth = 1.f;

    m_Render_Dsec.emplace_back(Render_Desc);

    return S_OK;
}

void CGraphic_Device::Set_RenderResource(_uInt iIndex)
{
    if (0 > iIndex || m_Render_Dsec.size() <= iIndex)
        return;

    m_CurViewWinow = iIndex;

    /*장치에 바인드해놓을 렌더 타겟들과 뎁스스텐실뷰를 세팅한다.* /
    /* 장치는 동시에 최대 8개의 렌더타겟을 들고 있을 수 있다. */
    /* OutMerge에다가 데이터를 넘겨준다.*/
    // 랜더 타겟과 깊이 스텐실 버퍼를 넘겨서 세팅
    ID3D11RenderTargetView * pRTVs[] = {
        m_Render_Dsec[iIndex].pRenderTargetView,
    };

    m_pDeviceContext->OMSetRenderTargets(1, pRTVs, m_Render_Dsec[iIndex].pDepthStencilView);
    m_pDeviceContext->RSSetViewports(1, &m_Render_Dsec[iIndex].ViewPort);
}

HRESULT CGraphic_Device::GetBackBuffer(_uInt iIndex, ID3D11Texture2D** ppOut)
{
    if (FAILED(m_Render_Dsec[iIndex].pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)ppOut)))
        return E_FAIL;

    return S_OK;
}

HRESULT CGraphic_Device::Initialize_SwapChain(const ENGINE_DESC& Engine_Desc, IDXGISwapChain** pOut)
{
    //Device와 DXGI 계층간의 상호작용을 가능하게 해주는 Com객체
    IDXGIDevice* pDevice = nullptr;
    m_pGraphicDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);

    //GPU 실제 장치를 표현하는 Com객체
    IDXGIAdapter* pAdapter = nullptr;
    pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

    //DXGI 객체생성을 가능하게해주는 Com객체
    IDXGIFactory* pFactory = nullptr;
    pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

    DXGI_SWAP_CHAIN_DESC SwapChain_Desc;
    ZeroMemory(&SwapChain_Desc, sizeof(SwapChain_Desc));

    SwapChain_Desc.BufferDesc.Width = Engine_Desc.iWinSizeX;
    SwapChain_Desc.BufferDesc.Height = Engine_Desc.iWinSizeY;
    SwapChain_Desc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChain_Desc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChain_Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChain_Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapChain_Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    SwapChain_Desc.SampleDesc.Count = 1;
    SwapChain_Desc.SampleDesc.Quality = 0;
    SwapChain_Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChain_Desc.BufferCount = 1;
    SwapChain_Desc.OutputWindow = Engine_Desc.hWnd;
    SwapChain_Desc.Windowed = Engine_Desc.bIsWindowed;
    SwapChain_Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    if (FAILED(pFactory->CreateSwapChain(m_pGraphicDevice, &SwapChain_Desc, pOut)))
        return E_FAIL;

    Safe_Release(pFactory);
    Safe_Release(pAdapter);
    Safe_Release(pDevice);
    return S_OK;
}

HRESULT CGraphic_Device::Initialize_RenderTarget(IDXGISwapChain* pSwapChan, ID3D11RenderTargetView** ppOut)
{
    ID3D11Texture2D* backBuffer = nullptr;
    if(FAILED(pSwapChan->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
        return E_FAIL;

    if (FAILED(m_pGraphicDevice->CreateRenderTargetView(backBuffer, nullptr, ppOut)))
        return E_FAIL;

    Safe_Release(backBuffer);
    return S_OK;
}

HRESULT CGraphic_Device::Initialize_DepthStencil(_uInt WinSizeX, _uInt WinSizeY, ID3D11DepthStencilView** ppOut)
{
    if (nullptr == m_pGraphicDevice)
        return E_FAIL;

    ID3D11Texture2D* pDepthStencilTex = nullptr;
   
    D3D11_TEXTURE2D_DESC Texture_Desc;
    ZeroMemory(&Texture_Desc, sizeof(D3D11_TEXTURE2D_DESC));

    //깊이 버퍼를 기록하기위한 텍스처를 만든다.
    Texture_Desc.Width = WinSizeX;      //깊이 버퍼의 사이즈는 랜더타겟 사이즈보다 크거나 같아야한다.
    Texture_Desc.Height = WinSizeY;
    Texture_Desc.MipLevels = 1;         //민맵 설정 0이면 만들기 1이면 안만들기
    Texture_Desc.ArraySize = 1;
    Texture_Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    //기본옵션 설정 이값은 다중샘플링 값이다.
    //창화면에의해서 같이 늘어날지 말지를 결정
    Texture_Desc.SampleDesc.Quality = 0;
    Texture_Desc.SampleDesc.Count = 1;

    Texture_Desc.Usage = D3D11_USAGE_DEFAULT; //동적 버퍼 정적 버퍼 개념이다. (정적으로선언)
    Texture_Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;//텍스쳐를 어떤 용도로 사용하나

    // 이 두개는 CPU 설정과 맵설정관한거다.
    // 그냥 기본값으로 설정하자.
    Texture_Desc.CPUAccessFlags = 0;
    Texture_Desc.MiscFlags = 0;

    //텍스처 생성함수
    if (FAILED(m_pGraphicDevice->CreateTexture2D(&Texture_Desc, nullptr, &pDepthStencilTex)))
        return E_FAIL;

    // 스텐실 뷰 생성 함수
    // DepthStencil텍스처는 Resource의 자식이기때문에 넣어짐
    if (FAILED(m_pGraphicDevice->CreateDepthStencilView(pDepthStencilTex, nullptr, ppOut)))
        return E_FAIL;

    Safe_Release(pDepthStencilTex);
    return S_OK;
}

CGraphic_Device* CGraphic_Device::Create(const ENGINE_DESC& Engine_Desc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
    CGraphic_Device* pGraphicDevice = new CGraphic_Device();
    if (FAILED(pGraphicDevice->Initialize_Grphic(Engine_Desc, ppDeviceOut, ppContextOut)))
    {
        Safe_Release(pGraphicDevice);
        MSG_BOX("CREATE FAIL : GRAPHIC DEVICE");
    }

    return pGraphicDevice;
}

void CGraphic_Device::Free()
{
    Safe_Release(m_pDeviceContext);
    for (auto& iter : m_Render_Dsec)
    {
        Safe_Release(iter.pDepthStencilView);
        Safe_Release(iter.pRenderTargetView);
        Safe_Release(iter.pSwapChain);
    }

#if defined(DEBUG) || defined(_DEBUG)
    ID3D11Debug* d3dDebug;
    HRESULT hr = m_pGraphicDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
    if (SUCCEEDED(hr))
    {
        OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
        OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
        OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");

        hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

        OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
        OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
        OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
    }
    if (d3dDebug != nullptr)            d3dDebug->Release();
#endif
    Safe_Release(m_pGraphicDevice);
}
