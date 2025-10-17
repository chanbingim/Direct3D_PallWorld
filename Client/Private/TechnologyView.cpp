#include "TechnologyView.h"

#include "GameInstance.h"
#include "TechSlotLine.h"

CTechnologyView::CTechnologyView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{
}

CTechnologyView::CTechnologyView(const CTechnologyView& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CTechnologyView::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTechnologyView::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    return S_OK;
}

void CTechnologyView::Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CTechnologyView::Late_Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);
}

HRESULT CTechnologyView::Render()
{
    return S_OK;
}

HRESULT CTechnologyView::ADD_Childs()
{
    _float fSlotCount = 6;
    _float3 vParentScale = m_pTransformCom->GetScale();
    CTechSlotLine::GAMEOBJECT_DESC TechSlotLineDesc = {};

    TechSlotLineDesc.pParent = this;
    TechSlotLineDesc.vScale = { vParentScale.x, vParentScale.y / fSlotCount, 0.f };
    _float3 vStartPos = { -vParentScale.x * 0.5f + TechSlotLineDesc.vScale.x * 0.5f,
                          -vParentScale.y * 0.5f + TechSlotLineDesc.vScale.y * 0.5f, 0.f };

    for (_uInt i = 0; i < fSlotCount; ++i)
    {
        TechSlotLineDesc.vPosition.y = vStartPos.y + TechSlotLineDesc.vScale.y * i;
        CTechSlotLine* pTechSlotLine = CTechSlotLine::Create(m_pGraphic_Device, m_pDeviceContext);

        pTechSlotLine->SetZOrder(m_iZOrder + 1);
        if (FAILED(pTechSlotLine->Initialize(&TechSlotLineDesc)))
            return E_FAIL;

        pTechSlotLine->SetTechLevel(i + 1);
        m_pTechSlotLine.push_back(pTechSlotLine);
        ADD_Child(pTechSlotLine);
    }
    return S_OK;
}

CTechnologyView* CTechnologyView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTechnologyView* pTechnologyView = new CTechnologyView(pDevice, pContext);
    if (FAILED(pTechnologyView->Initalize_Prototype()))
    {
        Safe_Release(pTechnologyView);
        MSG_BOX("CREATE FAIL : TECHNOLOGY VIEW");
    }
    return pTechnologyView;
}

CGameObject* CTechnologyView::Clone(void* pArg)
{
    CTechnologyView* pTechnologyView = new CTechnologyView(*this);
    if (FAILED(pTechnologyView->Initialize(pArg)))
    {
        Safe_Release(pTechnologyView);
        MSG_BOX("CLONE FAIL : TECHNOLOGY VIEW");
    }
    return pTechnologyView;
}

void CTechnologyView::Free()
{
    __super::Free();

    for (auto iter : m_pTechSlotLine)
        Safe_Release(iter);
}
