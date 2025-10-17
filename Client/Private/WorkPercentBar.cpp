#include "WorkPercentBar.h"

#include "GameInstance.h"

CWorkPercentBar::CWorkPercentBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CProgressBar(pDevice, pContext)
{
}

CWorkPercentBar::CWorkPercentBar(const CWorkPercentBar& rhs) :
    CProgressBar(rhs)
{
}

HRESULT CWorkPercentBar::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWorkPercentBar::Initialize(void* pArg)
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

void CWorkPercentBar::Update(_float fDeletaTime)
{
    //지오 메트리 셰이더로 퍼센트 구현해보기



}

void CWorkPercentBar::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CWorkPercentBar::Render()
{
    Apply_ConstantShaderResources();

    m_pShaderCom->Update_Shader(4);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

void CWorkPercentBar::SetPosition(_float3 vPosition)
{
    _float3 vParentPos = m_pParent->GetTransform()->GetPosition();
    _vector vPercentPos = XMLoadFloat3(&vParentPos) + XMLoadFloat3(&vPosition);
    XMStoreFloat3(&vPosition, vPercentPos);

    m_pTransformCom->SetPosition(vPosition);
}

HRESULT CWorkPercentBar::Apply_ConstantShaderResources()
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    m_pShader_Percent->SetRawValue(&m_fPercent, 0, sizeof(_float));
    m_pShader_Color->SetFloatVector((float*)&m_vColor);

    return S_OK;

}

HRESULT CWorkPercentBar::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Pell_PalSpherUI_PercentBar_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CWorkPercentBar* CWorkPercentBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWorkPercentBar* pWorkPercnetBar = new CWorkPercentBar(pDevice, pContext);
    if (FAILED(pWorkPercnetBar->Initalize_Prototype()))
    {
        Safe_Release(pWorkPercnetBar);
        MSG_BOX("CREATE FAIL : WORK PERCENT BAR");
    }
    return pWorkPercnetBar;
}

CGameObject* CWorkPercentBar::Clone(void* pArg)
{
    CWorkPercentBar* pWorkPercnetBar = new CWorkPercentBar(*this);
    if (FAILED(pWorkPercnetBar->Initialize(pArg)))
    {
        Safe_Release(pWorkPercnetBar);
        MSG_BOX("CLONE FAIL : WORK PERCENT BAR");
    }
    return pWorkPercnetBar;
}

void CWorkPercentBar::Free()
{
    __super::Free();
}
