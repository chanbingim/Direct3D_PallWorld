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
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

  
   
    return S_OK;
}

void CBackGround::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CBackGround::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CBackGround::Render()
{

    return S_OK;
}

HRESULT CBackGround::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBackGround::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();

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
    Safe_Release(m_pShaderCom);
}
