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


    VIEWER_DESC* ViewDesc = static_cast<VIEWER_DESC*>(pArg);
    if (FAILED(CreateViewTexture()))
        return E_FAIL;

    if (FAILED(CreateViewerCamera(ViewDesc->fWidth, ViewDesc->fHeight)))
        return E_FAIL;

    return S_OK;
}

void CViewer::Update(_float fDeletaTime)
{
    auto iCurLevelID = m_pGameInstance->GetCurrentLevel()->GetLevelID();
    const list<CGameObject*>* pObjects = m_pGameInstance->GetAllObejctToLayer(iCurLevelID, TEXT("Layer_GamePlay_Player"));
    
    SetViewObject(*(*pObjects).begin());
    _float3 vCameraPos = m_pViewObject->GetTransform()->GetPosition();
    _vector vUp = XMVector3Normalize(m_pViewObject->GetTransform()->GetUpVector()) * (m_pViewObject->GetTransform()->GetScale().y * 0.5f);
    _vector vLook = XMVector3Normalize(m_pViewObject->GetTransform()->GetLookVector()) * m_fCameraDistance;
    
    XMStoreFloat3(&vCameraPos, XMLoadFloat3(&vCameraPos) + vLook + vUp);
    SetViewCameraPosition(vCameraPos);
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

    m_pShader_Resource->SetResource(m_pViewTexture);
    return S_OK;
}

void CViewer::UpdateShaderResource(_uInt iRenderIndex)
{
}

HRESULT CViewer::CreateViewTexture()
{
    //Shader리소스로 생성해서 데이터를 보관한다.
    ID3D11Texture2D* pBackBuffer = nullptr;
    m_pGameInstance->GetBackBuffer(&pBackBuffer);

    D3D11_TEXTURE2D_DESC TexDesc = {};
    pBackBuffer->GetDesc(&TexDesc);

    TexDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

    ID3D11Texture2D* pTeuxtre2D = nullptr;
    if (FAILED(m_pGraphic_Device->CreateTexture2D(&TexDesc, nullptr, &pTeuxtre2D)))
        return E_FAIL;

    if (FAILED(m_pGraphic_Device->CreateRenderTargetView(pTeuxtre2D, nullptr, &m_pRenderTargetTex)))
        return E_FAIL;

    if (FAILED(m_pGraphic_Device->CreateShaderResourceView(pTeuxtre2D, nullptr, &m_pViewTexture)))
        return E_FAIL;

    TexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;//텍스쳐를 어떤 용도로 사용하나
    Safe_Release(pTeuxtre2D);

    if (FAILED(m_pGraphic_Device->CreateTexture2D(&TexDesc, nullptr, &pTeuxtre2D)))
        return E_FAIL;

    if (FAILED(m_pGraphic_Device->CreateDepthStencilView(pTeuxtre2D, nullptr, &m_pDepthStencil)))
        return E_FAIL;

    Safe_Release(pTeuxtre2D);
    Safe_Release(pBackBuffer);
    return S_OK;
}

HRESULT CViewer::CreateViewerCamera(_float fWidth, _float fHeight)
{
    CBaseCamera::CAMERA_DESC Desc;
    ZeroMemory(&Desc, sizeof(CBaseCamera::CAMERA_DESC));
    Desc.fFar = 100.f;
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
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetTex, vClearColor);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetTex, m_pDepthStencil);
    _float4x4 m_OldVeiwMat = m_pGameInstance->GetMatrix(MAT_STATE::VIEW);
    _float4x4 m_OldProjMat = m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION);

    m_pViewerCamera->Priority_Update(0.0f);

    m_pViewObject->Render();

    m_pGameInstance->SetMatrix(MAT_STATE::VIEW, m_OldVeiwMat);
    m_pGameInstance->SetMatrix(MAT_STATE::PROJECTION, m_OldProjMat);
    Safe_Release(m_pViewObject);

    m_pGameInstance->Set_RenderResource(0);
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

    Safe_Release(m_pViewTexture);
    Safe_Release(m_pRenderTargetTex);

    Safe_Release(m_pViewerCamera);
    Safe_Release(m_pDepthStencil);
    Safe_Release(m_pViewObject);
}
