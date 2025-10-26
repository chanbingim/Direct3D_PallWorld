#include "Renderer.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "UserInterface.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext),
    m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CRenderer::Initialize()
{
    _uInt		iNumViewports = { 1 };

    D3D11_VIEWPORT		Viewport{};
    m_pContext->RSGetViewports(&iNumViewports, &Viewport);

    /* 후처리 쉐이딩을 위한 렌더타겟들을 준비하낟. */
    /* Target_Diffuse */
    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.0f, 0.f, 0.f, 0.f))))
        return E_FAIL;

    /* Target_Normal */
    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 1.f))))
        return E_FAIL;

    /* Target_Depth */
    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.0f, 0.f, 0.f, 0.f))))
        return E_FAIL;

    /* Target_Shade */
    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
        return E_FAIL;

    /* Target_Specular */
    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 0.f))))
        return E_FAIL;

    /* Target_Blur */
    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.0f, 0.0f, 0.0f, 0.0f))))
        return E_FAIL;

    /* Target_Blur_X */
    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_X"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.0f, 0.0f, 0.0f, 0.0f))))
        return E_FAIL;

    /* Target_Mtrl_Specular */
    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Mtrl_Specular"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 0.f))))
        return E_FAIL;

    /* MRT_GameObjects */
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Mtrl_Specular"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
        return E_FAIL;

    /* MRT_LightAcc */
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
        return E_FAIL;

    /* MRT_Blur */
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur"), TEXT("Target_Blur"))))
        return E_FAIL;

    /* MRT_Blur_X */
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_X"), TEXT("Target_Blur_X"))))
        return E_FAIL;

    m_pShader = CShader::Create(m_pDevice, m_pContext, VTX_TEX::Elements, VTX_TEX::iNumElements, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"));
    if (nullptr == m_pShader)
        return E_FAIL;

    m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
    if (nullptr == m_pVIBuffer)
        return E_FAIL;

    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(Viewport.Width, Viewport.Height, 1.f));
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f));

#ifdef _DEBUG
    if (FAILED(m_pGameInstance->Ready_RenderTargetDebug(TEXT("Target_Diffuse"), 150.0f, 150.0f, 300.f, 300.f)))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Ready_RenderTargetDebug(TEXT("Target_Normal"), 150.0f, 450.0f, 300.f, 300.f)))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Ready_RenderTargetDebug(TEXT("Target_Shade"), 450.0f, 150.0f, 300.f, 300.f)))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Ready_RenderTargetDebug(TEXT("Target_Specular"), 450.0f, 450.0f, 300.f, 300.f)))
        return E_FAIL;
#endif

    return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDER eRenderGroup, CGameObject* pRenderObject)
{
    if (nullptr == pRenderObject)
        return E_FAIL;

    m_RenderObjects[ENUM_CLASS(eRenderGroup)].push_back(pRenderObject);
    Safe_AddRef(pRenderObject);

    return S_OK;
}

void CRenderer::Render()
{
    Render_Priority();

    Render_NonBlend();

    Render_LightAcc();
    Render_Blur();
    Render_Combined();
    Render_NonLight();
 
    Render_WorldUI();
    Render_Blend();
    DrawPosTex();

    Render_ScreenUI();

#ifdef _DEBUG
    if(m_bIsShowRenderTarget)
        Render_Debug();
#endif
}

#ifdef _DEBUG
void CRenderer::ToggleRenderDebug()
{
    m_bIsShowRenderTarget = !m_bIsShowRenderTarget;
}

HRESULT CRenderer::Add_DebugComponent(CComponent* pDebugCom)
{
    m_DebugComponents.push_back(pDebugCom);

    Safe_AddRef(pDebugCom);

    return S_OK;
}
#endif

void CRenderer::Render_Priority()
{
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::PRIORITY)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDER::PRIORITY)].clear();
}

void CRenderer::Render_NonBlend()
{
    /* Diffuse + Normal */
    if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
        return;

    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::NONBLEND)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDER::NONBLEND)].clear();

    if (FAILED(m_pGameInstance->End_MRT()))
        return;
}

void CRenderer::Render_LightAcc()
{
    /* Shade + Specular */
    if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc"))))
        return;

    m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
    m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
    m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
    m_pShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->GetInvMatrix(MAT_STATE::VIEW));
    m_pShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->GetInvMatrix(MAT_STATE::PROJECTION));

    _float4 vCamPos = {};
    XMStoreFloat4(&vCamPos, m_pGameInstance->GetCameraState(WORLDSTATE::POSITION));
    m_pShader->Bind_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4));

    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
        return;
    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
        return;

    if (FAILED(m_pGameInstance->Render_Lights(m_pShader, m_pVIBuffer)))
        return;

    if (FAILED(m_pGameInstance->End_MRT()))
        return;
}

void CRenderer::Render_Combined()
{
    m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
    m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
    m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
    m_pShader->Bind_RawValue("g_fFar", &m_pGameInstance->GetCameraINFO().y, sizeof(_float));

    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
        return;

    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
        return;
    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
        return;
    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
        return;
    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Blur"), m_pShader, "g_BlurTexture")))
        return;

    m_pShader->Update_Shader(3);
    m_pVIBuffer->Render_VIBuffer();
}

void CRenderer::Render_NonLight()
{
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::NONLIGHT)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDER::NONLIGHT)].clear();

}

void CRenderer::Render_Blur()
{
    if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur"))))
        return;

    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::BLUR)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDER::BLUR)].clear();

    if (FAILED(m_pGameInstance->End_MRT()))
        return;

   /* if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_X"))))
        return;

    m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
    m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
    m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

    if (FAILED(m_pGameInstance->Bind_RenderTarget(TEXT("Target_Blur"), m_pShader, "g_BlurTexture")))
        return;*/

   // m_pShader->Begin(4);

   //   m_pVIBuffer->Bind_Resources();

   // m_pVIBuffer->Render();

   // if (FAILED(m_pGameInstance->End_MRT()))
   //     return;
}

void CRenderer::Render_WorldUI()
{
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::WORLD_UI)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDER::WORLD_UI)].clear();
}

void CRenderer::Render_Blend()
{
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::BLEND)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDER::BLEND)].clear();
}

void CRenderer::Render_ScreenUI()
{
    m_RenderObjects[ENUM_CLASS(RENDER::SCREEN_UI)].sort([&](CGameObject* Src, CGameObject* Dest)
        {
            CUserInterface* SrcUI = dynamic_cast<CUserInterface*>(Src);
            CUserInterface* DestUI = dynamic_cast<CUserInterface*>(Dest);

            if (nullptr == SrcUI || nullptr == DestUI)
                return false;

            return SrcUI->GetZOrder() < DestUI->GetZOrder();
        });

    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::SCREEN_UI)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDER::SCREEN_UI)].clear();
}

#pragma region PostTex
void CRenderer::DrawPosTex()
{
    ID3D11Texture2D* pBackBuffer = nullptr;
    ID3D11Resource*  pPostBuffer = nullptr;
    auto PostTex = m_pGameInstance->GetPostBuffer(m_pGameInstance->GetRenderTargetNum());
    m_pGameInstance->GetBackBuffer(&pBackBuffer);

    if (PostTex && pBackBuffer)
    {
        PostTex->GetResource(&pPostBuffer);
        ////픽셀 값만 복사해오는 함수
        m_pContext->CopyResource(pPostBuffer, pBackBuffer);
        Safe_Release(pPostBuffer);
    }
    Safe_Release(pBackBuffer);
}


#pragma endregion
#ifdef _DEBUG
void CRenderer::Render_Debug()
{
    for (auto& pDebugCom : m_DebugComponents)
    {
        if (nullptr != pDebugCom)
            pDebugCom->Render();

        Safe_Release(pDebugCom);
    }
    m_DebugComponents.clear();

    if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return;
    if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return;

    /* 렌더타겟을 디버그로 직교투영을 통해 그려라. */
   /* if (FAILED(m_pGameInstance->Render_RenderTargetDebug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
        return;*/
    if (FAILED(m_pGameInstance->Render_RenderTargetDebug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
        return;
}
#endif // _DEBUG

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CRenderer* pRender = new CRenderer(pDevice, pContext);
    if (FAILED(pRender->Initialize()))
    {
        Safe_Release(pRender);
        MSG_BOX("CREATE FAIL : RENDERER");
    }
    return pRender;
}

void CRenderer::Free()
{
    for (auto& RenderObjects : m_RenderObjects)
    {
        for (auto& pRenderObject : RenderObjects)
            Safe_Release(pRenderObject);
        RenderObjects.clear();
    }

    Safe_Release(m_pShader);
    Safe_Release(m_pVIBuffer);

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
