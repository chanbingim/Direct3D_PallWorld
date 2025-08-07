#include "LoadingHUD.h"

#include "GameInstance.h"
#include "BackGround.h"

CLoadingHUD::CLoadingHUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CHeadUpDisplay(pDevice, pContext)
{
}

HRESULT CLoadingHUD::Initialize()
{
    if (FAILED(ADD_UserInterface()))
        return E_FAIL;

    return S_OK;
}

void CLoadingHUD::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

HRESULT CLoadingHUD::ADD_UserInterface()
{
    CUserInterface::GAMEOBJECT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CUserInterface::GAMEOBJECT_DESC));

    Desc.vScale = { g_iWinSizeX, g_iWinSizeY, 1.f };
    Desc.vPosition = { g_iHalfWinSizeX,  g_iHalfWinSizeY, 0.f };

    if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::LOADING), TEXT("Prototype_GameObject_LoadingUI"), TEXT("Loading_UI"), &Desc)))
        return E_FAIL;

    return S_OK;
}

CLoadingHUD* CLoadingHUD::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLoadingHUD* pHUD = new CLoadingHUD(pDevice, pContext);
    if (nullptr == pHUD)
    {
        Safe_Release(pHUD);
        MSG_BOX("CREATE FAIL : LOADING HUD");
    }

    return pHUD;
}

void CLoadingHUD::Free()
{
    __super::Free();
}
