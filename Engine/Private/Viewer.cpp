#include "Viewer.h"

#include "GameInstance.h"
#include "EditorCamera.h"
#include "Level.h"
#include "GameObject.h"

CViewer::CViewer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{
}

CViewer::CViewer(const CViewer& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CViewer::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CViewer::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;


    _float2 vScreenSize = m_pGameInstance->GetScreenSize();
    VIEWER_DESC* ViewDesc = static_cast<VIEWER_DESC*>(pArg);
    m_vViewerSize = { ViewDesc->fWidth, ViewDesc->fHeight };

    if (FAILED(CreateViewTexture(ViewDesc->fWidth, ViewDesc->fHeight)))
        return E_FAIL;

    if (FAILED(CreateViewerCamera(ViewDesc->fWidth, ViewDesc->fHeight)))
        return E_FAIL;

    /* Target_Blur_X */
    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Preview"), ViewDesc->fWidth, ViewDesc->fHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.0f, 0.0f, 0.0f, 0.0f))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_PreView"), TEXT("Target_Preview"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Ready_RenderTargetDebug(TEXT("Target_Preview"), vScreenSize.x - 450.0f, 450.0f, 300.f, 300.f)))
        return E_FAIL;

    return S_OK;
}

void CViewer::Update(_float fDeletaTime)
{

}

void CViewer::Late_Update(_float fDeletaTime)
{
   
}

HRESULT CViewer::Render()
{
    if(m_pViewObject)
        RenderObejct();

    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);

    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

void CViewer::SetViewCameraPosition(_float3 Pos)
{
    m_pViewerCamera->SetLocation(Pos);

    if (m_pViewObject)
    {
        _float3 vPos = m_pViewObject->GetTransform()->GetPosition();
        _vector vUp = XMVector3Normalize(m_pViewObject->GetTransform()->GetUpVector()) * (m_pViewObject->GetTransform()->GetScale().y * 0.5f);
        m_pViewerCamera->CameraLookAt(XMLoadFloat3(&vPos) + vUp);
    }
}

void CViewer::SetViewCameraFov(_float fFov)
{
    m_pViewerCamera->SetFov(fFov);
}

void CViewer::SetViewObject(CGameObject* pGameObject)
{
    m_pViewObject = pGameObject;
    Safe_AddRef(m_pViewObject);
}

HRESULT CViewer::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pShader_Resource = m_pShaderCom->GetVariable("g_Texture")->AsShaderResource();
    return S_OK;
}

HRESULT CViewer::Apply_ConstantShaderResources()
{
    if (FAILED(__super::Apply_ConstantShaderResources()))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Preview"), m_pShaderCom, "g_Texture")))
        return E_FAIL;
    return S_OK;
}

void CViewer::UpdateShaderResource(_uInt iRenderIndex)
{
}

HRESULT CViewer::CreateViewTexture(_uInt iSizeX, _uInt iSizeY)
{
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;

    ID3D11Texture2D* pDepthStencilTexture = nullptr;

    D3D11_TEXTURE2D_DESC	TextureDesc;
    ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

    /* 깊이 버퍼의 픽셀은 백버퍼의 픽셀과 갯수가 동일해야만 깊이 텍스트가 가능해진다. */
    /* 픽셀의 수가 다르면 아에 렌더링을 못함. */
    TextureDesc.Width = iSizeX;
    TextureDesc.Height = iSizeY;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.SampleDesc.Count = 1;

    /* 동적? 정적?  */
    TextureDesc.Usage = D3D11_USAGE_DEFAULT /* 정적 */;
    /* 추후에 어떤 용도로 바인딩 될 수 있는 View타입의 텍스쳐를 만들기위한 Texture2D입니까? */
    TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
        /*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
    TextureDesc.CPUAccessFlags = 0;
    TextureDesc.MiscFlags = 0;

    if (FAILED(m_pGraphic_Device->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
        return E_FAIL;


    if (FAILED(m_pGraphic_Device->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDepthStencil)))
        return E_FAIL;

    Safe_Release(pDepthStencilTexture);

    return S_OK;
    return S_OK;
}

HRESULT CViewer::CreateViewerCamera(_float fWidth, _float fHeight)
{
    CBaseCamera::CAMERA_DESC Desc;
    ZeroMemory(&Desc, sizeof(CBaseCamera::CAMERA_DESC));
    Desc.fFar = 200.f;
    Desc.fNear = 0.1f;
    Desc.fFov = XMConvertToRadians(60.f);
    Desc.vEye = { 0.f, 0.f, 1.f };

    //카메라를 생성해서 보관함
    m_pViewerCamera = CEditorCamera::Create(m_pGraphic_Device, m_pDeviceContext);
    if (nullptr == m_pViewerCamera)
        return E_FAIL;

    m_pViewerCamera->Initialize(&Desc);
    if (fWidth && fHeight)
        m_pViewerCamera->SetAspect(fWidth, fHeight);
    return S_OK;
}

void CViewer::RenderObejct()
{
    _float vClearColor[4] = {0.f, 0.f, 0.f, 0.f};
    m_pGameInstance->Begin_MRT(TEXT("MRT_PreView"), m_pDepthStencil);

    D3D11_VIEWPORT			ViewPortDesc;
    ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
    ViewPortDesc.TopLeftX = 0;
    ViewPortDesc.TopLeftY = 0;
    ViewPortDesc.Width = (_float)m_vViewerSize.x;
    ViewPortDesc.Height = (_float)m_vViewerSize.y;
    ViewPortDesc.MinDepth = 0.f;
    ViewPortDesc.MaxDepth = 1.f;
    m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);

    _float4x4 m_OldVeiwMat = m_pGameInstance->GetMatrix(MAT_STATE::VIEW);
    _float4x4 m_OldProjMat = m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION);

    m_pViewerCamera->Priority_Update(0.0f);
    m_pViewObject->Render();

    m_pGameInstance->SetMatrix(MAT_STATE::VIEW, m_OldVeiwMat);
    m_pGameInstance->SetMatrix(MAT_STATE::PROJECTION, m_OldProjMat);
    m_pGameInstance->End_MRT();

    ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
    ViewPortDesc.TopLeftX = 0;
    ViewPortDesc.TopLeftY = 0;
    ViewPortDesc.Width = m_pGameInstance->GetScreenSize().x;
    ViewPortDesc.Height = m_pGameInstance->GetScreenSize().y;
    ViewPortDesc.MinDepth = 0.f;
    ViewPortDesc.MaxDepth = 1.f;
    m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);
}

void CViewer::MouseHovering()
{
}

void CViewer::MouseButtonDwon()
{
}

void CViewer::MouseButtonPressed()
{
}

void CViewer::MouseButtonUp()
{
}

CViewer* CViewer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CViewer* pViewer = new CViewer(pDevice, pContext);
    if (FAILED(pViewer->Initalize_Prototype()))
    {
        Safe_Release(pViewer);
        MSG_BOX("CREATE FAIL : VIEWER");
    }

    return pViewer;
}

CUserInterface* CViewer::Clone(void* pArg)
{
    CViewer* pViewer = new CViewer(*this);
    if (FAILED(pViewer->Initialize(pArg)))
    {
        Safe_Release(pViewer);
        MSG_BOX("CLONE FAIL : VIEWER");
    }

    return pViewer;
}

void CViewer::Free()
{
    __super::Free();
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);

    Safe_Release(m_pViewerCamera);
    Safe_Release(m_pDepthStencil);
    Safe_Release(m_pViewObject);
}
