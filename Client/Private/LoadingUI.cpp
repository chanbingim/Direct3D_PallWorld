#include "LoadingUI.h"

#include "GameInstance.h"
#include "LoadingHUD.h"

CLoadingUI::CLoadingUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CLoadingUI::CLoadingUI(const CLoadingUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CLoadingUI::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLoadingUI::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    return S_OK;
}

void CLoadingUI::Update(_float fDeletaTime)
{
}

void CLoadingUI::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CLoadingUI::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CLoadingUI::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingBackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLoadingUI::ADD_Childs()
{
    auto pDefaultHUD = m_pGameInstance->GetCurrentHUD();
    if (nullptr == pDefaultHUD)
        return E_FAIL;

    auto pLoadingHUD = dynamic_cast<CLoadingHUD*>(pDefaultHUD);
    if (pLoadingHUD)
    {
        CUserInterface::GAMEOBJECT_DESC Desc = {};
        Desc.pParent = this;

        Desc.vScale = { 400.f, 300.f, 0.f };
        Desc.vPosition = { g_iHalfWinSizeX * 0.65f, g_iHalfWinSizeY * 0.7f, 0.f };
        if (FAILED(pLoadingHUD->Add_UserInterface(ENUM_CLASS(LEVEL::LOADING), TEXT("Prototype_GameObject_LoadingPellLogo"), TEXT("LoadingPellLogo"), &Desc)))
            return E_FAIL;

        Desc.vScale = { 30, 30.f, 0.f };
        Desc.vRotation = { 0.f, 0.f, 0.f };
        Desc.vPosition = { g_iHalfWinSizeX * 0.35f, g_iHalfWinSizeY * 0.92f, 0.f};
        if (FAILED(pLoadingHUD->Add_UserInterface(ENUM_CLASS(LEVEL::LOADING), TEXT("Prototype_GameObject_LoadingIcon"), TEXT("LoadingIcon"), &Desc)))
            return E_FAIL;
    }

    return S_OK;
}

CLoadingUI* CLoadingUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLoadingUI* pLoadingUI = new CLoadingUI(pDevice, pContext);
    if (FAILED(pLoadingUI->Initalize_Prototype()))
    {
        Safe_Release(pLoadingUI);
        MSG_BOX("CREATE FAIL : Loading UI");
    }

    return pLoadingUI;
}

CGameObject* CLoadingUI::Clone(void* pArg)
{
    CLoadingUI* pLoadingUI = new CLoadingUI(*this);
    if (FAILED(pLoadingUI->Initialize(pArg)))
    {
        Safe_Release(pLoadingUI);
        MSG_BOX("CLONE FAIL : Loading UI");
    }

    return pLoadingUI;
}

void CLoadingUI::Free()
{
    __super::Free();
}
