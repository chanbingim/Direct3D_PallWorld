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
    if (FAILED(Create_BlendState()))
        return E_FAIL;

    if (FAILED(Create_DepthStencilState()))
        return E_FAIL;

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

    Render_WorldUI();
    FLOAT blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
    m_pContext->OMSetBlendState(m_pAlphaBlendState, blendFactor, 0xffffffff);
    Render_Blend();
   
    Render_ScreenUI();
    m_pContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

HRESULT CRenderer::Create_BlendState()
{
    D3D11_BLEND_DESC desc = { 0 };
    desc.AlphaToCoverageEnable = FALSE;
    desc.IndependentBlendEnable = FALSE;

    D3D11_RENDER_TARGET_BLEND_DESC& RenderTarget = desc.RenderTarget[0];
    RenderTarget.BlendEnable = TRUE;
    RenderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    RenderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    RenderTarget.BlendOp = D3D11_BLEND_OP_ADD;
    RenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
    RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
    RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    if (FAILED(m_pDevice->CreateBlendState(&desc, &m_pAlphaBlendState)))
    {
        MSG_BOX("CREATE FAIL : BLEND STATE");
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CRenderer::Create_DepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC Desc;
    ZeroMemory(&Desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

   if (FAILED(m_pDevice->CreateDepthStencilState(&Desc, &m_pUIDepthStencilState)))
       return E_FAIL;

    return S_OK;
}

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
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::NONBLEND)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDER::NONBLEND)].clear();
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
  
    DrawPosTex();
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

    m_pContext->OMSetDepthStencilState(m_pUIDepthStencilState, 0);

    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDER::SCREEN_UI)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDER::SCREEN_UI)].clear();
    m_pContext->OMSetDepthStencilState(nullptr, 0);
}

#pragma region PostTex
void CRenderer::DrawPosTex()
{
    ID3D11Texture2D* pBackBuffer = nullptr;
    ID3D11Resource*  pPostBuffer = nullptr;
    auto PostTex = m_pGameInstance->GetPostBuffer(0);
    m_pGameInstance->GetBackBuffer(0, (ID3D11Texture2D**)&pBackBuffer);
   
    if (PostTex)
    {
        /* 원본 리소스를 얻어온다. */
        PostTex->GetResource(&pPostBuffer);
        //픽셀 값만 복사해오는 함수
        m_pContext->CopyResource(pPostBuffer, pBackBuffer);

        Safe_Release(pPostBuffer);
    }
    Safe_Release(pBackBuffer);
}
#pragma endregion

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

    Safe_Release(m_pAlphaBlendState);
    Safe_Release(m_pUIDepthStencilState);
    Safe_Release(m_pGameInstance);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
