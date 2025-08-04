#include "Logo_Menu.h"

#include "GameInstance.h"
#include "Logo_HUD.h"

CLogo_Menu::CLogo_Menu(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CBackGround(pGraphic_Device, pDeviceContext)
{
}

CLogo_Menu::CLogo_Menu(const CLogo_Menu& rhs) : 
    CBackGround(rhs)
{
}

HRESULT CLogo_Menu::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLogo_Menu::Initialize(void* pArg)
{
    if (FAILED(CUserInterface::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if(FAILED(Bind_ShaderCBuffer()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    return S_OK;
}

void CLogo_Menu::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

HRESULT CLogo_Menu::Render()
{
    Apply_ConstantShaderResources();

    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);

    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CLogo_Menu::ADD_Childs()
{    
    auto pDefaultHUD = m_pGameInstance->GetCurrentHUD();
    if (nullptr == pDefaultHUD)
        return E_FAIL;

    auto pLogo_HUD = dynamic_cast<CLogo_HUD*>(pDefaultHUD);
    if (pLogo_HUD)
    {
        CUserInterface::GAMEOBJECT_DESC Desc = {};
        Desc.pParent = this;
        Desc.vScale = { 100.f, 50.f, 0.f };
        Desc.vPosition = { 0.f, -100.f, 0.f };
        if (FAILED(pLogo_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LMenu_But_GameStart"), TEXT("LMenu_But_GameStart"), &Desc)))
              return E_FAIL;

        Desc.vPosition = { 640.f, 425.f, 0.f };
        if (FAILED(pLogo_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LMenu_But_GameEdit"), TEXT("LMenu_But_GameEdit"), &Desc)))
            return E_FAIL;

        Desc.vPosition = { 640.f, 500.f, 0.f };
        if (FAILED(pLogo_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LMenu_But_GameQuit"), TEXT("LMenu_But_GameQuit"), &Desc)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CLogo_Menu::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CLogo_Menu* CLogo_Menu::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CLogo_Menu* pLogoMenu = new CLogo_Menu(pGraphic_Device, pDeviceContext);
    if (FAILED(pLogoMenu->Initalize_Prototype()))
    {
        Safe_Release(pLogoMenu);
        MSG_BOX("CREATE FAIL : LOGO MENU");
    }

    return pLogoMenu;
}

CGameObject* CLogo_Menu::Clone(void* pArg)
{
    CGameObject* pLogoMenu = new CLogo_Menu(*this);
    if (FAILED(pLogoMenu->Initialize(pArg)))
    {
        Safe_Release(pLogoMenu);
        MSG_BOX("CLONE FAIL : LOGO MENU");
    }

    return pLogoMenu;
}

void CLogo_Menu::Free()
{
    __super::Free();
}