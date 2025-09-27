#include "ItemSlotIcon.h"

#include "GameInstance.h"

CItemSlotIcon::CItemSlotIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{
}

CItemSlotIcon::CItemSlotIcon(const CItemSlotIcon& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CItemSlotIcon::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemSlotIcon::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    ITEM_SLOT_ICON_DESC* pItemIconDesc = static_cast<ITEM_SLOT_ICON_DESC*>(pArg);
    m_pParentTransform = pItemIconDesc->pParentTransform;

    return S_OK;
}

void CItemSlotIcon::Update(_float fDeletaTime)
{
    
}

void CItemSlotIcon::Late_Update(_float fDeletaTime)
{
    m_pTransformCom->SetPosition(m_pParentTransform->GetPosition());
    if (m_pItemIcon)
        m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CItemSlotIcon::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);

    m_pItemIcon->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

void CItemSlotIcon::SetTexture(const CTexture* pTexture)
{
    m_pItemIcon = pTexture;
}

HRESULT CItemSlotIcon::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();

    return S_OK;
}

HRESULT CItemSlotIcon::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CItemSlotIcon* CItemSlotIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItemSlotIcon* pItemSlotIcon = new CItemSlotIcon(pDevice, pContext);
    if (FAILED(pItemSlotIcon->Initalize_Prototype()))
    {
        Safe_Release(pItemSlotIcon);
        MSG_BOX("CREATE FAIL : ITEM SLOT ICON");
    }
    return pItemSlotIcon;
}

CUserInterface* CItemSlotIcon::Clone(void* pArg)
{
    CItemSlotIcon* pItemSlotIcon = new CItemSlotIcon(*this);
    if (FAILED(pItemSlotIcon->Initialize(pArg)))
    {
        Safe_Release(pItemSlotIcon);
        MSG_BOX("CLONE FAIL : ITEM SLOT ICON");
    }
    return pItemSlotIcon;
}

void CItemSlotIcon::Free()
{
    __super::Free();
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
}