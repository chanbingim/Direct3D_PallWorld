#include "BattlePell.h"

#include "GameInstance.h"
#include "GamePlayHUD.h"

CBattlePell::CBattlePell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CBattlePell::CBattlePell(const CBattlePell& rhs) :
    CBackGround(rhs)
{
}

HRESULT CBattlePell::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBattlePell::Initialize(void* pArg)
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

void CBattlePell::Update(_float fDeletaTime)
{
}

void CBattlePell::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CBattlePell::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CBattlePell::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_Pell_UI"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBattlePell::ADD_Childs()
{
    auto pDefaultHUD = m_pGameInstance->GetCurrentHUD();
    if (nullptr == pDefaultHUD)
        return E_FAIL;

    auto pInGame_HUD = dynamic_cast<CGamePlayHUD*>(pDefaultHUD);
    if (pInGame_HUD)
    {
        CBackGround::GAMEOBJECT_DESC Desc = {};
        Desc.pParent = this;
       
        //Frame
        Desc.vScale = { 80.f, 80.f, 0.f };
        Desc.vPosition = { 0.f, 0.f, 0.f };
        if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Battle_PellFrame_UI"), TEXT("BattlePellFrame"), &Desc)))
            return E_FAIL;

        //InfoUI
        Desc.vScale = { 100.f, 30.f, 0.f };
        Desc.vPosition = { 0.f, 60.f, 0.f };
        if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Battle_PellInfo_UI"), TEXT("PellInfo"), &Desc)))
            return E_FAIL;
    }

    return S_OK;
}

CBattlePell* CBattlePell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBattlePell* pBattlePell = new CBattlePell(pDevice, pContext);
    if (FAILED(pBattlePell->Initalize_Prototype()))
    {
        Safe_Release(pBattlePell);
        MSG_BOX("CREATE FAIL : BATTLE PELL UI");
    }

    return pBattlePell;
}

CGameObject* CBattlePell::Clone(void* pArg)
{
    CBattlePell* pBattlePell = new CBattlePell(*this);
    if (FAILED(pBattlePell->Initialize(pArg)))
    {
        Safe_Release(pBattlePell);
        MSG_BOX("CLONE FAIL : BATTLE PELL UI");
    }

    return pBattlePell;
}

void CBattlePell::Free()
{
    __super::Free();
}
