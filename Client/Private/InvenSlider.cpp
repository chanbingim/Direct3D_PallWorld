#include "InvenSlider.h"

#include "GameInstance.h"
#include "InvenSliderButton.h"

#include "PlayerManager.h"

CInvenSlider::CInvenSlider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSlider(pDevice, pContext)
{
}

CInvenSlider::CInvenSlider(const CInvenSlider& rhs) :
    CSlider(rhs)
{
}

HRESULT CInvenSlider::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CInvenSlider::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(CreateSliderButton()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    m_vColor = { 1.f,1.f,1.f, 0.5f };
    return S_OK;
}

void CInvenSlider::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CInvenSlider::Late_Update(_float fDeletaTime)
{

}

HRESULT CInvenSlider::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pSliderBut->Render();
    return S_OK;
}

HRESULT CInvenSlider::Bind_ShaderResources()
{
    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pShader_Color = m_pShaderCom->GetVariable("g_Color")->AsVector();

    return S_OK;
}

HRESULT CInvenSlider::Apply_ConstantShaderResources()
{
    __super::Apply_ConstantShaderResources();
    m_pShader_Color->SetFloatVector((float*)&m_vColor);

    return S_OK;
}

void CInvenSlider::MouseButtonDwon()
{

    
}

void CInvenSlider::MouseButtonPressed()
{
    __super::MouseButtonPressed();
}

void CInvenSlider::MouseButtonUp()
{
}

HRESULT CInvenSlider::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Pell_HeatlhBar_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CInvenSlider::CreateSliderButton()
{
    m_pSliderBut = CInvenSliderButton::Create(m_pGraphic_Device, m_pDeviceContext);
    CInvenSliderButton::GAMEOBJECT_DESC Desc = {};

    Desc.pParent = this;
    Desc.vScale = { 20.f, 50.f, 0.f };
    Desc.vPosition = { 0.f, -m_pTransformCom->GetScale().y * 0.5f , 0.f};
    if (FAILED(m_pSliderBut->Initialize(&Desc)))
        return E_FAIL;
    return S_OK;
}

CInvenSlider* CInvenSlider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInvenSlider* pInvenSlider = new CInvenSlider(pDevice, pContext);
    if (FAILED(pInvenSlider->Initalize_Prototype()))
    {
        Safe_Release(pInvenSlider);
        MSG_BOX("CREATE FAIL : INVEN SLIDER");
    }
    return pInvenSlider;
}

CUserInterface* CInvenSlider::Clone(void* pArg)
{
    CInvenSlider* pInvenSlider = new CInvenSlider(*this);
    if (FAILED(pInvenSlider->Initialize(pArg)))
    {
        Safe_Release(pInvenSlider);
        MSG_BOX("CLONE FAIL : INVEN SLIDER");
    }
    return pInvenSlider;
}

void CInvenSlider::Free()
{
    __super::Free();
}
