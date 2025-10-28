#include "PlayerView.h"

#include "GameInstance.h"
#include "PreviewModel.h"

CPlayerView::CPlayerView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CViewer(pDevice, pContext)
{
}

CPlayerView::CPlayerView(const CPlayerView& rhs) :
    CViewer(rhs)
{
}

HRESULT CPlayerView::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerView::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    
    m_fCameraDistance = 2.f;
    m_fPlayerViewFov = 80.f;

    m_pViewerCamera->SetFov(m_fPlayerViewFov);
    m_pViewerCamera->SetAspect(m_vViewerSize.x, m_vViewerSize.y);

    m_pViewerCamera->CameraLookAt(XMVectorSet(0.f, 0.f, 0.f, 1.f));
    m_fPlayerViewMinMax = { 60.f, 120.f };
    m_bIsMouseEvent = true;
    return S_OK;
}

void CPlayerView::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    CUserInterface::Update(fDeletaTime);
    if (m_pViewObject)
    {
        m_pViewObject->Priority_Update(fDeletaTime);
        m_pViewObject->Update(fDeletaTime);

        _float3 vModelPos = m_pViewObject->GetTransform()->GetPosition();
        _vector vCalModelPos = XMLoadFloat3(&vModelPos);

        _vector vModelLook = m_pViewObject->GetTransform()->GetLookVector();
        
        _float3 vCameraPos = {};
        _vector vCalCameraPos = XMVectorZero() + vModelLook * m_fCameraDistance;

        XMStoreFloat3(&vCameraPos, vCalCameraPos);
        m_pViewerCamera->GetTransform()->SetPosition(vCameraPos);
        m_pViewerCamera->CameraLookAt(-vModelLook);
    }
}

void CPlayerView::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    
    if (m_pViewObject)
        m_pViewObject->Late_Update(fDeletaTime);
}

HRESULT CPlayerView::Render()
{
    if (m_pViewObject)
        RenderObejct();

    if (FAILED(Apply_ConstantShaderResources()))
        return E_FAIL;

    m_pShaderCom->Update_Shader(2);
    m_pVIBufferCom->Render_VIBuffer();


    return S_OK;
}

HRESULT CPlayerView::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerView::Apply_ConstantShaderResources()
{
   if (FAILED(__super::Apply_ConstantShaderResources()))
        return E_FAIL;

 
 
    return S_OK;
}

void CPlayerView::MouseHovering()
{
    LONG MousePosZ = m_pGameInstance->GetMouseAxis(2);
    if (0 < MousePosZ)
    {
        m_fPlayerViewFov -= 10.f;
        if (m_fPlayerViewMinMax.x >= m_fPlayerViewFov)
            m_fPlayerViewFov = m_fPlayerViewMinMax.x;
    }
    else if (0 > MousePosZ)
    {
        m_fPlayerViewFov += 10.f;
        if (m_fPlayerViewMinMax.y <= m_fPlayerViewFov)
            m_fPlayerViewFov = m_fPlayerViewMinMax.y;
    }

    m_pViewerCamera->SetFov(m_fPlayerViewFov);
}

void CPlayerView::MouseButtonDwon()
{
}

void CPlayerView::MouseButtonPressed()
{
}

void CPlayerView::MouseButtonUp()
{
}

HRESULT CPlayerView::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    CPreviewModel::GAMEOBJECT_DESC PreviewModelDesc = {};
    PreviewModelDesc.vScale = { 1.f, 1.f, 1.f };
    PreviewModelDesc.vPosition = { 0.f, -PreviewModelDesc.vScale.x, 0.f };

    m_pViewObject = CPreviewModel::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(m_pViewObject->Initialize(&PreviewModelDesc)))
        return E_FAIL;

    return S_OK;
}

CPlayerView* CPlayerView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerView* pPlayerView = new CPlayerView(pDevice, pContext);
    if (FAILED(pPlayerView->Initalize_Prototype()))
    {
        Safe_Release(pPlayerView);
        MSG_BOX("CREATE FAIL : PLAYER VIEW");
    }

    return pPlayerView;
}

CUserInterface* CPlayerView::Clone(void* pArg)
{
    CPlayerView* pPlayerView = new CPlayerView(*this);
    if (FAILED(pPlayerView->Initialize(pArg)))
    {
        Safe_Release(pPlayerView);
        MSG_BOX("CLONE FAIL : PLAYER VIEW");
    }

    return pPlayerView;
}

void CPlayerView::Free()
{
    __super::Free();
}
