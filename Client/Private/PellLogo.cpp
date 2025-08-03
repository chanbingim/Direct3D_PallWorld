#include "PellLogo.h"

#include "GameInstance.h"

CPellLogo::CPellLogo(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CBackGround(pGraphic_Device, pDeviceContext)
{
}

CPellLogo::CPellLogo(const CPellLogo& rhs) :
    CBackGround(rhs)
{
}

HRESULT CPellLogo::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;
}

HRESULT CPellLogo::Initialize(void* pArg)
{
    if (FAILED(CUserInterface::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderCBuffer()))
        return E_FAIL;

    CreateAlphaBlendState();

    return S_OK;
}

void CPellLogo::CreateAlphaBlendState()
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

    if (FAILED(m_pGraphic_Device->CreateBlendState(&desc, &m_pBlendState)))
    {
        MSG_BOX("CREATE FAIL : BLEND STATE");
        return;
    }
}

void CPellLogo::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

HRESULT CPellLogo::Render()
{
    //FLOAT BLEND_Factor[4] = { 0.f, 0.f, 0.f, 0.f };
    //m_pDeviceContext->OMSetBlendState(m_pBlendState, BLEND_Factor, 0xffffffff);
    //__super::Render();

    _matrix Idenity = XMMatrixIdentity();

    static_cast<LPD3D11EFFECTMATRIXVARIABLE>(m_pWorldMat)->SetMatrix(reinterpret_cast<float*>(&m_pTransformCom->GetWorldMat()));
    static_cast<LPD3D11EFFECTMATRIXVARIABLE>(m_pViewMat)->SetMatrix(reinterpret_cast<float*>(&Idenity));
    static_cast<LPD3D11EFFECTMATRIXVARIABLE>(m_pProjMat)->SetMatrix(reinterpret_cast<float*>(&m_ProjMatrix));

    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 0);

    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CPellLogo::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_GameLogo"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CPellLogo* CPellLogo::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CPellLogo* pPellLogo = new CPellLogo(pGraphic_Device, pDeviceContext);
    if (FAILED(pPellLogo->Initalize_Prototype()))
    {
        Safe_Release(pPellLogo);
        MSG_BOX("CREATE FAIL : PELL LOGO");
    }

    return pPellLogo;
}

CGameObject* CPellLogo::Clone(void* pArg)
{
    CGameObject* pPellLogo = new CPellLogo(*this);
    if (FAILED(pPellLogo->Initialize(pArg)))
    {
        Safe_Release(pPellLogo);
        MSG_BOX("CLONE FAIL : PELL LOGO");
    }

    return pPellLogo;
}

void CPellLogo::Free()
{
    __super::Free();

    Safe_Release(m_pBlendState);
}
