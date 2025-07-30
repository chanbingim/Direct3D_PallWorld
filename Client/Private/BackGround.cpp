#include "BackGround.h"

#include "GameInstance.h"

CBackGround::CBackGround(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CUserInterface(pGraphic_Device, pDeviceContext)
{
}

CBackGround::CBackGround(const CBackGround& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CBackGround::Initalize_Prototype()
{


    return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(SettingShader()))
        return E_FAIL;

    D3D11_INPUT_ELEMENT_DESC VertexDesc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    m_pGraphic_Device->CreateInputLayout(VertexDesc, 2, vs_Blob->GetBufferPointer(), vs_Blob->GetBufferSize(), &InputLayOut);
    return S_OK;
}

void CBackGround::Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CBackGround::Render()
{
    m_pDeviceContext->IASetInputLayout(InputLayOut);
    m_pDeviceContext->VSSetShader(pVertexShader, nullptr, 0);
    m_pDeviceContext->PSSetShader(pPixelShader, nullptr, 0);
    m_pTextureCom->SetTexture(0, 0);

    m_pVIBufferCom->Render_VIBuffer();

   
    return S_OK;
}

HRESULT CBackGround::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBackGround::SettingShader()
{
    const _uInt compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

    D3DCompileFromFile(L"TestShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", compileFlag, 0, &vs_Blob, nullptr);
    if(FAILED(m_pGraphic_Device->CreateVertexShader(vs_Blob->GetBufferPointer(), vs_Blob->GetBufferSize(), nullptr, &pVertexShader)))
       return E_FAIL;

    D3DCompileFromFile(L"TestShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", compileFlag, 0, &ps_Blob, nullptr);
    if (FAILED(m_pGraphic_Device->CreatePixelShader(ps_Blob->GetBufferPointer(), ps_Blob->GetBufferSize(), nullptr, &pPixelShader)))
        return E_FAIL;

    return S_OK;
}

CBackGround* CBackGround::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CBackGround* pBackGround = new CBackGround(pGraphic_Device, pDeviceContext);
    if (FAILED(pBackGround->Initalize_Prototype()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("CREATE FAIL : BACK GROUND");
    }
    return pBackGround;
}

CGameObject* CBackGround::Clone(void* pArg)
{
    CBackGround* pBackGround = new CBackGround(*this);
    if (FAILED(pBackGround->Initialize(pArg)))
    {
        Safe_Release(pBackGround);
        MSG_BOX("CLONE FAIL : BACK GROUND");
    }
    return pBackGround;
}

void CBackGround::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);


    Safe_Release(InputLayOut);
    Safe_Release(pVertexShader);
    Safe_Release(pPixelShader);
    Safe_Release(vs_Blob);
    Safe_Release(ps_Blob);
}
