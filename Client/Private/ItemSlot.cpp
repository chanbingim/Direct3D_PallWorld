#include "ItemSlot.h"

#include "GameInstance.h"

CItemSlot::CItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSlotBase(pDevice, pContext, SLOT_TYPE::ITEM)
{
}

CItemSlot::CItemSlot(const CItemSlot& rhs) :
    CSlotBase(rhs)
{
}

HRESULT CItemSlot::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemSlot::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CItemSlot::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CItemSlot::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CItemSlot::Render()
{
    if (FAILED(Apply_ConstantShaderResources()))
        return E_FAIL;

    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

void CItemSlot::SwapSlot(CSlotBase* To)
{
}

void CItemSlot::UseSlot(void* pArg)
{
}

void CItemSlot::MouseHoverEnter()
{
}

void CItemSlot::MouseHovering()
{
}

void CItemSlot::MouseHoverExit()
{
}

void CItemSlot::MouseButtonDwon()
{
}

void CItemSlot::MouseButtonPressed()
{
}

void CItemSlot::MouseButtonUp()
{
}

HRESULT CItemSlot::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_Category_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CItemSlot* CItemSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItemSlot* pItemSlot = new CItemSlot(pDevice, pContext);
    if (FAILED(pItemSlot->Initalize_Prototype()))
    {
        Safe_Release(pItemSlot);
        MSG_BOX("CREATE FAIL : ITEM SLOT");
    }
    return pItemSlot;
}

CUserInterface* CItemSlot::Clone(void* pArg)
{
    CItemSlot* pItemSlot = new CItemSlot(*this);
    if (FAILED(pItemSlot->Initialize(pArg)))
    {
        Safe_Release(pItemSlot);
        MSG_BOX("CLONE FAIL : ITEM SLOT");
    }
    return pItemSlot;
}

void CItemSlot::Free()
{
    __super::Free();
}
