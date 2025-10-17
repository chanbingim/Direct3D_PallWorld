#include "PlayerView.h"

#include "GameInstance.h"

#include "PlayerManager.h"
#include "Player.h"

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

    auto pPlayer = CPlayerManager::GetInstance()->GetCurrentPlayer();

    m_pViewObject = pPlayer->FindPartObject(TEXT("Player_Animator"));
    Safe_AddRef(m_pViewObject);

    m_fCameraDistance = 15.f;

    m_fPlayerViewFov = 80.f;
    m_pViewerCamera->SetFov(m_fPlayerViewFov);
    m_fPlayerViewMinMax = { 60.f, 120.f };
    m_bIsMouseEvent = true;
    return S_OK;
}

void CPlayerView::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
    CUserInterface::Update(fDeletaTime);

   
}

void CPlayerView::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    
    if (m_pViewObject)
        RenderObejct();
}

HRESULT CPlayerView::Render()
{
    if (FAILED(Apply_ConstantShaderResources()))
        return E_FAIL;

    m_pShaderCom->Update_Shader(0);
    m_pDeviceContext->PSSetShaderResources(0, 1, &m_pViewTexture);

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
