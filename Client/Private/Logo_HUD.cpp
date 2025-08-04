#include "Logo_HUD.h"

#include "GameInstance.h"
#include "BackGround.h"

CLogo_HUD::CLogo_HUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CHeadUpDisplay(pDevice, pContext)
{
}

HRESULT CLogo_HUD::Initialize()
{
    if (FAILED(ADD_UserInterface()))
        return E_FAIL;

    return S_OK;
}

void CLogo_HUD::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

HRESULT CLogo_HUD::ADD_UserInterface()
{
    CUserInterface::GAMEOBJECT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CUserInterface::GAMEOBJECT_DESC));

    Desc.vScale = { g_iWinSizeX, g_iWinSizeY, 1.f };
    Desc.vPosition = { g_iHalfWinSizeX,  g_iHalfWinSizeY, 0.f };

    if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMenu"), TEXT("Main_BackGround"), &Desc)))
        return E_FAIL;

    Desc.vScale = { g_iWinSizeX * 0.6f, g_iWinSizeY * 0.5f, 1.f };
    Desc.vPosition = { g_iHalfWinSizeX, g_iHalfWinSizeY - 100.f, 0.f };

    if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_PellLogo"), TEXT("Main_BackGroundLogo"), &Desc)))
        return E_FAIL;

    return S_OK;
}

CLogo_HUD* CLogo_HUD::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLogo_HUD* pHUD = new CLogo_HUD(pDevice, pContext);
    if (nullptr == pHUD)
    {
        Safe_Release(pHUD);
        MSG_BOX("CREATE FAIL : LOGO HUD");
    }

    return pHUD;
}

void CLogo_HUD::Free()
{
    __super::Free();
}
