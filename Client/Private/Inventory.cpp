#include "Inventory.h"

#include "GameInstance.h"
#include "ItemSlot.h"

CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CInventory::CInventory(const CInventory& rhs) : 
    CBackGround(rhs)
{
}

HRESULT CInventory::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;
    
    m_SlotCount = { 5, 9 };
    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CInventory::Update(_float fDeletaTime)
{
    for (auto iter : m_pChildList)
        iter->Update(fDeletaTime);
}

void CInventory::Late_Update(_float fDeletaTime)
{
    for (auto iter : m_pChildList)
        iter->Late_Update(fDeletaTime);
}

HRESULT CInventory::Render()
{
    if (FAILED(Apply_ConstantShaderResources()))
        return E_FAIL;

    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    for (auto iter : m_pChildList)
        iter->Render();
    return S_OK;
}

HRESULT CInventory::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    m_pInven_Color = m_pShaderCom->GetVariable("g_Color")->AsVector();
    return S_OK;
}

HRESULT CInventory::Apply_ConstantShaderResources()
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_ViewMatrix));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_ProjMatrix));

    _float     vColor[4] = {0.2f, 0.2f, 0.2f, 0.2f};
    m_pInven_Color->SetFloatVector(vColor);
    return S_OK;
}

HRESULT CInventory::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_Category_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory::ADD_Childs()
{
    CItemSlot::ITEM_SLOT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CItemSlot::ITEM_SLOT_DESC));

    Desc.pParent = this;
    Desc.vScale = {40.f, 40.f, 0.f};

    _float3 vParentScale = m_pTransformCom->GetScale();
    _float3 vStartPos = { -vParentScale.x * 0.5f,  -vParentScale.y * 0.5f, 0.f};
    for (_uInt i = 0; i < m_SlotCount.y; ++i)
    {
        Desc.vPosition.y = vStartPos.y;
        for (_uInt j = 0; j < m_SlotCount.x; ++j)
        {
            Desc.vPosition.x = vStartPos.x;
            CItemSlot* pItemSlot = CItemSlot::Create(m_pGraphic_Device, m_pDeviceContext);
            if (nullptr == pItemSlot)
                return E_FAIL;

            if(FAILED(pItemSlot->Initialize(&Desc)))
                return E_FAIL;

            m_pItemSlot.push_back(pItemSlot);
            ADD_Child(pItemSlot);
        }
    }



    return S_OK;
}

CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInventory* pInventory = new CInventory(pDevice, pContext);
    if (FAILED(pInventory->Initalize_Prototype()))
    {
        Safe_Release(pInventory);
        MSG_BOX("CREATE FAIL : INVENTORY");
    }
    return pInventory;
}

CUserInterface* CInventory::Clone(void* pArg)
{
    CInventory* pInventory = new CInventory(*this);
    if (FAILED(pInventory->Initialize(pArg)))
    {
        Safe_Release(pInventory);
        MSG_BOX("CLONE FAIL : INVENTORY");
    }
    return pInventory;
}

void CInventory::Free()
{
    __super::Free();

    for (auto& pSlot : m_pItemSlot)
        Safe_Release(pSlot);

    m_pItemSlot.clear();
}
