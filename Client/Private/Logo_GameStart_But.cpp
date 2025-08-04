#include "Logo_GameStart_But.h"

#include "GameInstance.h"
#include "LogoLevel.h"

CLogo_GameStart_But::CLogo_GameStart_But(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CBackGround(pGraphic_Device, pDeviceContext)
{
}

CLogo_GameStart_But::CLogo_GameStart_But(const CLogo_GameStart_But& rhs) :
    CBackGround(rhs)
{
}

HRESULT CLogo_GameStart_But::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLogo_GameStart_But::Initialize(void* pArg)
{
    if (FAILED(CUserInterface::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderCBuffer()))
        return E_FAIL;


    m_bIsMouseEvent = true;
    return S_OK;
}

void CLogo_GameStart_But::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CLogo_GameStart_But::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CLogo_GameStart_But::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);

    if(!m_bIsHover)
        m_pTextureCom->SetTexture(0, 0);
    else
        m_pTextureCom->SetTexture(0, 1);

    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

void CLogo_GameStart_But::MouseHoverEnter()
{

}

void CLogo_GameStart_But::MouseHoverExit()
{

}

void CLogo_GameStart_But::MouseButtonDwon()
{
    auto DefalutLevel = m_pGameInstance->GetCurrentLevel();
    if (nullptr == DefalutLevel)
        return;

    auto pLogoLevel = dynamic_cast<CLogoLevel*>(DefalutLevel);
    if (pLogoLevel)
        pLogoLevel->NextLevelChange(true);
}

HRESULT CLogo_GameStart_But::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_LMenu_GameStart_But"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CLogo_GameStart_But* CLogo_GameStart_But::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CLogo_GameStart_But* pLogo_GameStart_But = new CLogo_GameStart_But(pGraphic_Device, pDeviceContext);
    if (FAILED(pLogo_GameStart_But->Initalize_Prototype()))
    {
        Safe_Release(pLogo_GameStart_But);
        MSG_BOX("CREATE FAIL : LOGO GAME START BUTTON");
    }
    return pLogo_GameStart_But;
}

CGameObject* CLogo_GameStart_But::Clone(void* pArg)
{
    CLogo_GameStart_But* pLogo_GameStart_But = new CLogo_GameStart_But(*this);
    if (FAILED(pLogo_GameStart_But->Initialize(pArg)))
    {
        Safe_Release(pLogo_GameStart_But);
        MSG_BOX("CLONE FAIL : LOGO GAME START BUTTON");
    }
    return pLogo_GameStart_But;
}

void CLogo_GameStart_But::Free()
{
    __super::Free();
}
