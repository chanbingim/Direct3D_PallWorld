#include "InGameMenu.h"

#include "GameInstance.h"

CInGameMenu::CInGameMenu(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CBackGround(pGraphic_Device, pDeviceContext)
{
}

CInGameMenu::CInGameMenu(const CInGameMenu& rhs) :
    CBackGround(rhs)
{
}

HRESULT CInGameMenu::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CInGameMenu::Initialize(void* pArg)
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

void CInGameMenu::Update(_float fDeletaTime)
{
}

void CInGameMenu::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CInGameMenu::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

void CInGameMenu::SetActive(_bool flag)
{
    m_bIsActive = flag;
}

_bool CInGameMenu::IsActive()
{
    return m_bIsActive;
}

HRESULT CInGameMenu::ADD_Childs()
{
    // 버튼 만들기
    return S_OK;
}

HRESULT CInGameMenu::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellInfo_Background"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CInGameMenu* CInGameMenu::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CInGameMenu* pInGameMenu = new CInGameMenu(pGraphic_Device, pDeviceContext);
    if (FAILED(pInGameMenu->Initalize_Prototype()))
    {
        Safe_Release(pInGameMenu);
        MSG_BOX("CREATE FAIL : IN GAME MENU");
    }
    return pInGameMenu;
}

CGameObject* CInGameMenu::Clone(void* pArg)
{
    CInGameMenu* pInGameMenu = new CInGameMenu(*this);
    if (FAILED(pInGameMenu->Initialize(pArg)))
    {
        Safe_Release(pInGameMenu);
        MSG_BOX("CLONE FAIL : IN GAME MENU");
    }
    return pInGameMenu;
}

void CInGameMenu::Free()
{
    __super::Free();
}
