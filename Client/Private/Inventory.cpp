#include "Inventory.h"

#include "GameInstance.h"

#include "ItemSlot.h"
#include "InvenSlider.h"

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
    
    m_SlotCount = { 5, 12 };
    m_SlotSize = 40.f;

    m_pViewItemSlot = new list<CItemSlot*>[m_SlotCount.y];

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CInventory::Update(_float fDeletaTime)
{
    _float3 vStartPos = { (_float)m_SlotViewSize.left, (_float)m_SlotViewSize.top, 0.f };
    _float fOffset = -m_pIvenSlider->GetCurPercent() * 100.f;
    _uInt iRowIndex = (m_pIvenSlider->GetCurPercent() * m_SlotCount.y);

    for (_uInt i = 0; i < (_uInt)m_SlotCount.y; ++i)
    {
        if (m_SlotCount.y <= iRowIndex)
            iRowIndex -= m_SlotCount.y;

        for (_uInt j = 0; j < (_uInt)m_SlotCount.x; ++j)
        {
            _uInt iIndex = _uInt(i * m_SlotCount.x + j);
            _float2 SlotPos = m_pItemSlot[iIndex]->GetViewPos();
            SlotPos.y = vStartPos.y + (m_SlotSize + 10) * i + fOffset;
            m_pItemSlot[iIndex]->SetLocation({ SlotPos.x ,SlotPos.y ,0.f });
           
            RECT rc{};
            if (IntersectRect(&rc, &m_SlotViewSize, &m_pItemSlot[iIndex]->GetRectSize()))
            {
                m_pViewItemSlot[iRowIndex].push_back(m_pItemSlot[iIndex]);
            }
            else
            {
                SlotPos.y -= m_SlotViewSize.bottom;
                SlotPos.y += vStartPos.y - (m_SlotSize + 10);
                m_pItemSlot[iIndex]->SetLocation({ SlotPos.x, SlotPos.y, 0.f });
                if (IntersectRect(&rc, &m_SlotViewSize, &m_pItemSlot[iIndex]->GetRectSize()))
                {
                    m_pViewItemSlot[0].push_back(m_pItemSlot[iIndex]);
                }
            }
        }
        iRowIndex++;
    }

    _uInt iSlotStartNum = m_SlotCount.x * (-fOffset);
    for (_uInt i = 0; i < (_uInt)m_SlotCount.y; ++i)
    {
        for (auto iter : m_pViewItemSlot[i])
        {
            iter->SetSlotNumber(iSlotStartNum++);
            iter->Update(fDeletaTime);
        }
    }
   
    for (auto iter : m_pChildList)
        iter->Update(fDeletaTime);
}

void CInventory::Late_Update(_float fDeletaTime)
{
    for (auto iter : m_pChildList)
        iter->Late_Update(fDeletaTime);

    for (_uInt i = 0; i < (_uInt)m_SlotCount.y; ++i)
    {
        for (auto pSlot : m_pViewItemSlot[i])
            pSlot->Late_Update(fDeletaTime);
    }

}

HRESULT CInventory::Render()
{
    for (auto iter : m_pChildList)
        iter->Render();

    for (_uInt i = 0; i < (_uInt)m_SlotCount.y; ++i)
    {
        for (auto pSlot : m_pViewItemSlot[i])
            pSlot->Render();

        m_pViewItemSlot[i].clear();
    }
      
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

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory::ADD_Childs()
{
    if (FAILED(ADD_Slider()))
        return E_FAIL;

    const RECT& SliderRect = m_pIvenSlider->GetRectSize();
    m_SlotViewSize = { m_UISize.left,  SliderRect.top,
                       m_UISize.right, SliderRect.bottom };

    m_fRowSpacing = (SliderRect.bottom - SliderRect.top) / m_SlotCount.y;

    if (FAILED(ADD_Slot()))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory::ADD_Slot()
{
    CItemSlot::ITEM_SLOT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CItemSlot::ITEM_SLOT_DESC));

    Desc.pParent = nullptr;
    Desc.vScale = { m_SlotSize, m_SlotSize, 0.f };

    _float3 vParentScale = m_pTransformCom->GetScale();
    _float3 vStartPos = { (_float)m_SlotViewSize.left + 50, (_float)m_SlotViewSize.top, 0.f };
    for (_uInt i = 0; i < m_SlotCount.y; ++i)
    {
        for (_uInt j = 0; j < m_SlotCount.x; ++j)
        {
            Desc.vPosition.x = vStartPos.x + (Desc.vScale.x + 10) * j;
            CItemSlot* pItemSlot = CItemSlot::Create(m_pGraphic_Device, m_pDeviceContext);
            if (nullptr == pItemSlot)
                return E_FAIL;

            if (FAILED(pItemSlot->Initialize(&Desc)))
                return E_FAIL;

            m_pItemSlot.push_back(pItemSlot);
        }
    }
    return S_OK;
}

HRESULT CInventory::ADD_Slider()
{
    m_pIvenSlider = CInvenSlider::Create(m_pGraphic_Device, m_pDeviceContext);
    if (nullptr == m_pIvenSlider)
        return E_FAIL;

    CInvenSlider::SLIDER_DESC SliderDesc;
    ZeroMemory(&SliderDesc, sizeof(CInvenSlider::SLIDER_DESC));
    SliderDesc.pParent = this;
    SliderDesc.vScale = {20.f, 400.f, 0.f};
    SliderDesc.vPosition = { m_pTransformCom->GetScale().x * 0.4f, -m_pTransformCom->GetScale().y * 0.05f, 0.f };
    SliderDesc.eType = AXIS::VERTICAL;

    if (FAILED(m_pIvenSlider->Initialize(&SliderDesc)))
        return E_FAIL;

    ADD_Child(m_pIvenSlider);
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

    Safe_Release(m_pIvenSlider);
    Safe_Delete_Array(m_pViewItemSlot);
}
