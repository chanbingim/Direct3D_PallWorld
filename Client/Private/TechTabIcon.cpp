#include "TechTabIcon.h"

#include "GameInstance.h"

CTechTabIcon::CTechTabIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CTechTabIcon::CTechTabIcon(const CTechTabIcon& rhs) :
    CBackGround(rhs)
{
}

HRESULT CTechTabIcon::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;
    return S_OK;
}

HRESULT CTechTabIcon::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CTechTabIcon::Update(_float fDeletaTime)
{

}

void CTechTabIcon::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechTabIcon::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, m_iTechTexIndex);
    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

HRESULT CTechTabIcon::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Texture"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CTechTabIcon* CTechTabIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTechTabIcon* pTechTabIcon = new CTechTabIcon(pDevice, pContext);
    if (FAILED(pTechTabIcon->Initalize_Prototype()))
    {
        Safe_Release(pTechTabIcon);
        MSG_BOX("CREATE FAIL : TECH TAB ICON");
    }
    return pTechTabIcon;
}

CGameObject* CTechTabIcon::Clone(void* pArg)
{
    CTechTabIcon* pTechTabIcon = new CTechTabIcon(*this);
    if (FAILED(pTechTabIcon->Initialize(pArg)))
    {
        Safe_Release(pTechTabIcon);
        MSG_BOX("CLONE FAIL : TECH TAB ICON");
    }
    return pTechTabIcon;
}

void CTechTabIcon::Free()
{
    __super::Free();
}
