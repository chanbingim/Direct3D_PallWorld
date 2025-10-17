#include "InvenSliderButton.h"

#include "GameInstance.h"

CInvenSliderButton::CInvenSliderButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSliderButton(pDevice, pContext)
{
}

CInvenSliderButton::CInvenSliderButton(const CInvenSliderButton& rhs) :
    CSliderButton(rhs)
{
}

HRESULT CInvenSliderButton::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CInvenSliderButton::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    m_vColor = {1.f, 1.f, 1.f, 0.4f};
    m_bIsMouseEvent = false;
    return S_OK;
}

void CInvenSliderButton::Update(_float fDeletaTime)
{
}

void CInvenSliderButton::Late_Update(_float fDeletaTime)
{
}

HRESULT CInvenSliderButton::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

HRESULT CInvenSliderButton::Bind_ShaderResources()
{
    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pShader_Color = m_pShaderCom->GetVariable("g_Color")->AsVector();

    return S_OK;
}

HRESULT CInvenSliderButton::Apply_ConstantShaderResources()
{
    __super::Apply_ConstantShaderResources();
    m_pShader_Color->SetFloatVector((float*)&m_vColor);
    return S_OK;
}

HRESULT CInvenSliderButton::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Pell_HeatlhBar_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CInvenSliderButton* CInvenSliderButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInvenSliderButton* pInvSliderBut = new CInvenSliderButton(pDevice, pContext);
    if (FAILED(pInvSliderBut->Initalize_Prototype()))
    {
        Safe_Release(pInvSliderBut);
        MSG_BOX("CREATE FAIL : INV SLIDER BUTTON");
    }
    return pInvSliderBut;
}

CUserInterface* CInvenSliderButton::Clone(void* pArg)
{
    CInvenSliderButton* pInvSliderBut = new CInvenSliderButton(*this);
    if (FAILED(pInvSliderBut->Initialize(pArg)))
    {
        Safe_Release(pInvSliderBut);
        MSG_BOX("CLONE FAIL : INV SLIDER BUTTON");
    }
    return pInvSliderBut;
}

void CInvenSliderButton::Free()
{
    __super::Free();
}
