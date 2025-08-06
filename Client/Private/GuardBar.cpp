#include "GuardBar.h"

#include "GameInstance.h"

CGuardBar::CGuardBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CProgressBar(pDevice, pContext)
{
}

CGuardBar::CGuardBar(const CGuardBar& rhs) :
    CProgressBar(rhs)
{
}

HRESULT CGuardBar::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CGuardBar::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    return S_OK;
}

void CGuardBar::Update(_float fDeletaTime)
{

}

void CGuardBar::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CGuardBar::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    //m_pShaderCom->Update_Shader(1);
    //m_pTextureCom->SetTexture(0, 0);
    //m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CGuardBar::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_GuardBar_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CGuardBar* CGuardBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGuardBar* pGuardBar = new CGuardBar(pDevice, pContext);
    if (FAILED(pGuardBar->Initalize_Prototype()))
    {
        Safe_Release(pGuardBar);
        MSG_BOX("CREATE FAIL : Guard Bar");
    }

    return pGuardBar;
}

CGameObject* CGuardBar::Clone(void* pArg)
{
    CGuardBar* pGuardBar = new CGuardBar(*this);
    if (FAILED(pGuardBar->Initialize(pArg)))
    {
        Safe_Release(pGuardBar);
        MSG_BOX("CREATE FAIL : Guard Bar");
    }

    return pGuardBar;
}

void CGuardBar::Free()
{
    __super::Free();
}
