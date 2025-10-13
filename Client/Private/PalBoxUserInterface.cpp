#include "PalBoxUserInterface.h"

#include "PalBox.h"
#include "PalInvenListUI.h"
#include "PalBoxListUI.h"
#include "WorkPalListUI.h"

CPalBoxUserInterface::CPalBoxUserInterface(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{
}

CPalBoxUserInterface::CPalBoxUserInterface(const CPalBoxUserInterface& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CPalBoxUserInterface::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalBoxUserInterface::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    m_iZOrder = 3;
    return S_OK;
}

void CPalBoxUserInterface::Update(_float fDeletaTime)
{
    if (VISIBILITY::HIDDEN == m_eVisible)
        return;

    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);

}

void CPalBoxUserInterface::Late_Update(_float fDeletaTime)
{
    if (VISIBILITY::HIDDEN == m_eVisible)
        return;

    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);
}

HRESULT CPalBoxUserInterface::Render()
{
    return S_OK;
}

void CPalBoxUserInterface::SetViewPalBox(CPalBox* pPalBox)
{

}

HRESULT CPalBoxUserInterface::ADD_Childs()
{
    CGameObject::GAMEOBJECT_DESC pObjectDesc = {};

    pObjectDesc.vScale = { 400, 400.f, 0.f };
    pObjectDesc.vPosition = { g_iHalfWinSizeX,  g_iHalfWinSizeY -pObjectDesc.vScale.y * 0.5f, 0.f };
    m_pPalBoxList = CPalBoxListUI::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pPalBoxList->SetZOrder(m_iZOrder + 1);
    if (FAILED(m_pPalBoxList->Initialize(&pObjectDesc)))
        return E_FAIL;

    pObjectDesc.vScale = {400, 130.f, 0.f};
    pObjectDesc.vPosition = { g_iHalfWinSizeX -(pObjectDesc.vPosition.x *0.5f + pObjectDesc.vScale.x * 0.5f + 10.f), pObjectDesc.vPosition.y, 0.f};
    m_pPalInvenList = CPalInvenListUI::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pPalInvenList->SetZOrder(m_iZOrder + 1);
    if (FAILED(m_pPalInvenList->Initialize(&pObjectDesc)))
        return E_FAIL;

    pObjectDesc.vScale = { 200, 500.f, 0.f };
    pObjectDesc.vPosition = { g_iHalfWinSizeX,  g_iHalfWinSizeY + pObjectDesc.vScale.y * 0.5f + pObjectDesc.vScale.y * 0.5f, 0.f };
    m_pWorkPalList = CWorkPalListUI::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pWorkPalList->SetZOrder(m_iZOrder + 1);
    if (FAILED(m_pWorkPalList->Initialize(&pObjectDesc)))
        return E_FAIL;

    ADD_Child(m_pPalBoxList);
    ADD_Child(m_pWorkPalList);
    ADD_Child(m_pPalInvenList);
    return S_OK;
}

CPalBoxUserInterface* CPalBoxUserInterface::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPalBoxUserInterface* pPalBoxUserInterface = new CPalBoxUserInterface(pDevice, pContext);
    if (FAILED(pPalBoxUserInterface->Initalize_Prototype()))
    {
        Safe_Release(pPalBoxUserInterface);
        MSG_BOX("CREATE FAIL : Pal Box UserInterface");
    }
    return pPalBoxUserInterface;
}

CGameObject* CPalBoxUserInterface::Clone(void* pArg)
{
    CPalBoxUserInterface* pPalBoxUserInterface = new CPalBoxUserInterface(*this);
    if (FAILED(pPalBoxUserInterface->Initialize(pArg)))
    {
        Safe_Release(pPalBoxUserInterface);
        MSG_BOX("CLONE FAIL : Pal Box UserInterface");
    }
    return pPalBoxUserInterface;
}

void CPalBoxUserInterface::Free()
{
    __super::Free();

    Safe_Release(m_pPalBoxList);
    Safe_Release(m_pPalInvenList);
    Safe_Release(m_pWorkPalList);
}
