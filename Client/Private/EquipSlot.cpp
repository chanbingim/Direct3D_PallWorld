#include "EquipSlot.h"

#include "GameInstance.h"

CEquipSlot::CEquipSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSlotBase(pDevice, pContext, SLOT_TYPE::EQUIP)
{
}

CEquipSlot::CEquipSlot(const CEquipSlot& rhs) :
    CSlotBase(rhs)
{
}

HRESULT CEquipSlot::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEquipSlot::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CEquipSlot::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CEquipSlot::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CEquipSlot::Render()
{
    if (FAILED(Apply_ConstantShaderResources()))
        return E_FAIL;

    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

void CEquipSlot::SwapSlot(CSlotBase* To)
{
}

void CEquipSlot::UseSlot(void* pArg)
{
}

void CEquipSlot::MouseHoverEnter()
{
}

void CEquipSlot::MouseHovering()
{
}

void CEquipSlot::MouseHoverExit()
{
}

void CEquipSlot::MouseButtonDwon()
{
}

void CEquipSlot::MouseButtonPressed()
{
}

void CEquipSlot::MouseButtonUp()
{
}

HRESULT CEquipSlot::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_Category_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CEquipSlot* CEquipSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEquipSlot* pEquipSlot = new CEquipSlot(pDevice, pContext);
    if (FAILED(pEquipSlot->Initalize_Prototype()))
    {
        Safe_Release(pEquipSlot);
        MSG_BOX("CREATE FAIL : EQUIP SLOT");
    }
    return pEquipSlot;
}

CUserInterface* CEquipSlot::Clone(void* pArg)
{
    CEquipSlot* pEquipSlot = new CEquipSlot(*this);
    if (FAILED(pEquipSlot->Initialize(pArg)))
    {
        Safe_Release(pEquipSlot);
        MSG_BOX("CLONE FAIL : EQUIP SLOT");
    }
    return pEquipSlot;
}

void CEquipSlot::Free()
{
    __super::Free();
}
