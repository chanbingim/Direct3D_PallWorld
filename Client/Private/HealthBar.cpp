#include "HealthBar.h"

#include "GameInstance.h"

CHealthBar::CHealthBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CProgressBar(pDevice, pContext)
{
}

CHealthBar::CHealthBar(const CHealthBar& rhs) :
    CProgressBar(rhs)
{
}

HRESULT CHealthBar::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CHealthBar::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CHealthBar::Update(_float fDeletaTime)
{

}

void CHealthBar::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CHealthBar::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);

    m_pTextureCom->SetTexture(0, 0);
    m_pTextureCom->SetTexture(1, 1);

    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

HRESULT CHealthBar::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_HeatlhBar_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CHealthBar* CHealthBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHealthBar* pHealthBar = new CHealthBar(pDevice, pContext);
    if (FAILED(pHealthBar->Initalize_Prototype()))
    {
        Safe_Release(pHealthBar);
        MSG_BOX("CREATE FAIL : Health Bar");
    }

    return pHealthBar;
}

CGameObject* CHealthBar::Clone(void* pArg)
{
    CHealthBar* pHealthBar = new CHealthBar(*this);
    if (FAILED(pHealthBar->Initialize(pArg)))
    {
        Safe_Release(pHealthBar);
        MSG_BOX("CLONE FAIL : Health Bar");
    }

    return pHealthBar;
}

void CHealthBar::Free()
{
    __super::Free();
}
