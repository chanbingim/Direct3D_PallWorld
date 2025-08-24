#include "Viewer.h"

#include "GameInstance.h"
#include "EditorCamera.h"
#include "Level.h"

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

    if (FAILED(CreateViewTexture()))
        return E_FAIL;

    if (FAILED(CreateViewerCamera()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CViewer::Update(_float fDeletaTime)
{
}

void CViewer::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
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
        m_pViewerCamera->CameraLookAt(XMLoadFloat3(&vPos));
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

    return S_OK;
}

HRESULT CViewer::Apply_ConstantShaderResources()
{
    if (FAILED(__super::Apply_ConstantShaderResources()))
        return E_FAIL;

    return S_OK;
}

HRESULT CViewer::CreateViewTexture()
{
    //Shader리소스로 생성해서 데이터를 보관한다.
    D3D11_TEXTURE2D_DESC TexDesc = {};
    ZeroMemory(&TexDesc, sizeof(D3D11_TEXTURE2D_DESC));

    TexDesc.Width = 800;
    TexDesc.Height = 600;
    TexDesc.Usage = D3D11_USAGE_DEFAULT;
    TexDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
    TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    TexDesc.SampleDesc.Count = 1;
    TexDesc.SampleDesc.Quality = 0;
    TexDesc.ArraySize = 1;
    TexDesc.MipLevels = 1;
    TexDesc.MiscFlags = 0;
    TexDesc.CPUAccessFlags = 0;
    
    if(FAILED(m_pGraphic_Device->CreateTexture2D(&TexDesc, nullptr, &m_pViewTexture)))
        return E_FAIL;

    return S_OK;
}

HRESULT CViewer::CreateViewerCamera()
{
    //카메라를 생성해서 보관함
    m_pViewerCamera = CEditorCamera::Create(m_pGraphic_Device, m_pDeviceContext);
    if (nullptr == m_pViewerCamera)
        return E_FAIL;

    return S_OK;
}

void CViewer::RenderObejct()
{
    m_pViewObject->Render();
    Safe_Release(m_pViewObject);
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

    Safe_Release(m_pViewTexture);
}
