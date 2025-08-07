#include "LoadingIcon.h"

#include "GameInstance.h"

CLoadingIcon::CLoadingIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CLoadingIcon::CLoadingIcon(const CLoadingIcon& rhs) : 
    CBackGround(rhs)
{
}

HRESULT CLoadingIcon::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLoadingIcon::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;


    auto vScale = m_pTransformCom->GetScale();
    vScale.x -= 10.f;
    vScale.y -= 10.f;
    m_pFrontTransform->SetScale(vScale);
    m_pFrontTransform->SetRotation(m_pTransformCom->GetRotation());
    m_pFrontTransform->SetPosition(m_pTransformCom->GetPosition());

    m_eType = OBJECT_TYPE::STATIC;
    m_iZOrder = 1;
    return S_OK;
}

void CLoadingIcon::Update(_float fDeletaTime)
{
    m_fRadius += fDeletaTime * m_fSpeed;
      
    if (m_fRadius >= 360.f)
    {
        m_bIsRotate = !m_bIsRotate;
        m_fRadius = 0.f;
    }

    _float3 vRotate = {0.f, 0.f,  XMConvertToRadians(m_fRadius)};
    if (!m_bIsRotate)
        m_pTransformCom->SetRotation(vRotate);
    else
        m_pFrontTransform->SetRotation(vRotate);
}

void CLoadingIcon::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CLoadingIcon::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    Aplly_FrontObejctShaderResources();
    m_pShaderCom->Update_Shader(1);
    m_pFrontTexture->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CLoadingIcon::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingIconBack"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    // Front Icon Com
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingIconFront"), TEXT("FrontTexture_Com"), (CComponent**)&m_pFrontTexture)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), TEXT("FrontTransform_Com"), (CComponent**)&m_pFrontTransform)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLoadingIcon::Aplly_FrontObejctShaderResources()
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pFrontTransform->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_ViewMatrix));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_ProjMatrix));

    return S_OK;
}

CLoadingIcon* CLoadingIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLoadingIcon* pLoadingIcon = new CLoadingIcon(pDevice, pContext);
    if (FAILED(pLoadingIcon->Initalize_Prototype()))
    {
        Safe_Release(pLoadingIcon);
        MSG_BOX("CREATE FAIL : LOADING ICON");
    }

    return pLoadingIcon;
}

CGameObject* CLoadingIcon::Clone(void* pArg)
{
    CLoadingIcon* pLoadingIcon = new CLoadingIcon(*this);
    if (FAILED(pLoadingIcon->Initialize(pArg)))
    {
        Safe_Release(pLoadingIcon);
        MSG_BOX("CLONE FAIL : LOADING ICON");
    }

    return pLoadingIcon;
}

void CLoadingIcon::Free()
{
    __super::Free();

    Safe_Release(m_pFrontTexture);
    Safe_Release(m_pFrontTransform);
}
