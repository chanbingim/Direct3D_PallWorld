#include "RenderTarget.h"

#include "Shader.h"
#include "VIBuffer_Rect.h"

CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget::Initialize(_uInt iSizeX, _uInt iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    D3D11_TEXTURE2D_DESC            TextureDesc{};

    TextureDesc.Width = iSizeX;
    TextureDesc.Height = iSizeY;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = ePixelFormat;

    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.SampleDesc.Count = 1;

    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.CPUAccessFlags = 0;
    TextureDesc.MiscFlags = 0;

    // 랜더 타겟을 만들기 위해 우선 2D Txture를 만든다.
    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
        return E_FAIL;

    // 2D 텍스처를 통해서 랜더타겟을 바인딩하기위한 용도로 만든다.
    if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
        return E_FAIL;

    // 랜더타겟을 바인딩하기위한 용도 뿐만아니라 빛연산 및 후처리를 하기위해서
    // 셰이더로 정보를 넘기기 위한 셰이더 텍슽처를 만든다.
    if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
        return E_FAIL;

    m_vClearColor = vClearColor;

    return S_OK;
}

HRESULT CRenderTarget::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
    if (FAILED(pShader->Bind_SRV(pConstantName, m_pSRV)))
        return E_FAIL;

    return S_OK;
}

void CRenderTarget::Clear()
{
    m_pContext->ClearRenderTargetView(m_pRTV, reinterpret_cast<const _float*>(&m_vClearColor));

}

#ifdef _DEBUG
HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
    _uInt		iNumViewports = { 1 };

    D3D11_VIEWPORT		Viewport{};
    m_pContext->RSGetViewports(&iNumViewports, &Viewport);

    // 직교로 그리기 위해서
    // 뷰 투영을 곱해서 -1, 1 ~ 1, -1 사이의 값으로 변환 해둔다.
    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
    m_WorldMatrix._11 = fSizeX;
    m_WorldMatrix._22 = fSizeY;
    m_WorldMatrix._41 = fX - Viewport.Width * 0.5f;
    m_WorldMatrix._42 = -fY + Viewport.Height * 0.5f;

    return S_OK;
}

void CRenderTarget::Render_Debug(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
    if (FAILED(pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return;

    if (FAILED(pShader->Bind_SRV("g_Texture", m_pSRV)))
        return;

    pShader->Update_Shader(0);
    pVIBuffer->Render_VIBuffer();
}
#endif // _DEBUG

CRenderTarget* CRenderTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt iSizeX, _uInt iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    CRenderTarget* pRenderTarget = new CRenderTarget(pDevice, pContext);
    if (FAILED(pRenderTarget->Initialize(iSizeX, iSizeY, ePixelFormat, vClearColor)))
    {
        Safe_Release(pRenderTarget);
        MSG_BOX("CREATE FAIL : RENDER TARGET");
    }
    return pRenderTarget;
}

void CRenderTarget::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    
    Safe_Release(m_pTexture2D);
    Safe_Release(m_pRTV);
    Safe_Release(m_pSRV);
}
