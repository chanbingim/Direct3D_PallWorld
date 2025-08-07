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

    return S_OK;
}

HRESULT CPellLogo::Initialize(void* pArg)
{
    if (FAILED(CUserInterface::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    m_iZOrder = 1;
    return S_OK;
}

void CPellLogo::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

}

HRESULT CPellLogo::Render()
{
    Apply_ConstantShaderResources();

    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 0);

    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CPellLogo::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_GameLogo"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
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
}
