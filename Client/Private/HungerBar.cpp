#include "HungerBar.h"

#include "GameInstance.h"

CHungerBar::CHungerBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CProgressBar(pDevice, pContext)
{
}

CHungerBar::CHungerBar(const CHungerBar& rhs) :
    CProgressBar(rhs)
{
}

HRESULT CHungerBar::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CHungerBar::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CHungerBar::Update(_float fDeletaTime)
{
}

void CHungerBar::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CHungerBar::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 1);
    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

HRESULT CHungerBar::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_HeatlhBar_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CHungerBar* CHungerBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHungerBar* pHungerBar = new CHungerBar(pDevice, pContext);
    if (FAILED(pHungerBar->Initalize_Prototype()))
    {
        Safe_Release(pHungerBar);
        MSG_BOX("CREATE FAIL : HUNGER BAR");
    }
    return pHungerBar;
}

CGameObject* CHungerBar::Clone(void* pArg)
{
    CHungerBar* pHungerBar = new CHungerBar(*this);
    if (FAILED(pHungerBar->Initialize(pArg)))
    {
        Safe_Release(pHungerBar);
        MSG_BOX("CLONE FAIL : HUNGER BAR");
    }
    return pHungerBar;
}

void CHungerBar::Free()
{
    __super::Free();
}
