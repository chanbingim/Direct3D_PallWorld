#include "CreateMenu.h"
#include "GameInstance.h"

#include "TechSelectView.h"
#include "TechCategoryUI.h"

CCreateMenu::CCreateMenu(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CUserInterface(pGraphic_Device, pDeviceContext)
{
}

CCreateMenu::CCreateMenu(const CCreateMenu& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CCreateMenu::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CCreateMenu::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_CategoryButton()))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    m_iZOrder = 3;
    return S_OK;
}

void CCreateMenu::Update(_float fDeletaTime)
{
    if (false == m_bIsActive)
        return;

    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CCreateMenu::Late_Update(_float fDeletaTime)
{
    if (false == m_bIsActive)
        return;

    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);
}

HRESULT CCreateMenu::Render()
{
    return S_OK;
}

_bool CCreateMenu::IsActive()
{
    return m_bIsActive;
}

void CCreateMenu::SetActive(_bool bFlag)
{
    m_bIsActive = bFlag;
}

HRESULT CCreateMenu::ADD_Childs()
{
    _float3 vParentScale = m_pTransformCom->GetScale();
    CTechSelectView::GAMEOBJECT_DESC pTechSelectDesc = {};
    pTechSelectDesc.pParent = this;

    pTechSelectDesc.vScale = { vParentScale.x * 0.7f, vParentScale.y * 0.7f, 0.f };
    m_pCharacterView = CTechSelectView::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pCharacterView->SetZOrder(m_iZOrder + 1);
    if (FAILED(m_pCharacterView->Initialize(&pTechSelectDesc)))
        return E_FAIL;

    ADD_Child(m_pCharacterView);
    return S_OK;
}

HRESULT CCreateMenu::ADD_CategoryButton()
{
    _float3 vParentScale = m_pTransformCom->GetScale();
    CTechCategoryUI* pCategorybut = nullptr;

    // 버튼 만들기
    CTechCategoryUI::TECH_CATEGORY_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 170.f, 80.f, 0.f };

    _float ButtonPosX = g_iHalfWinSizeX * 0.5f + Desc.vScale.x * 0.5f;
    _float ButtonPosY = g_iHalfWinSizeY * 0.93f - Desc.vScale.y * 0.5f;
    /* CharacterInfo Button */
    Desc.CategoryType = TECH_TYPE::ARCHITECTURE;
    Desc.vPosition = { -ButtonPosX, -ButtonPosY , 0.f };
    pCategorybut = CTechCategoryUI::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(pCategorybut->Initialize(&Desc)))
        return E_FAIL;
    pCategorybut->SetZOrder(m_iZOrder + 1 );
    m_CategoryButton.push_back(pCategorybut);
    ADD_Child(pCategorybut);

    /* CharacterInfo Button */
    Desc.CategoryType = TECH_TYPE::PELL;
    Desc.vPosition = { -ButtonPosX + Desc.vScale.x, -ButtonPosY, 0.f };
    pCategorybut = CTechCategoryUI::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(pCategorybut->Initialize(&Desc)))
        return E_FAIL;
    pCategorybut->SetZOrder(m_iZOrder + 1);
    m_CategoryButton.push_back(pCategorybut);
    ADD_Child(pCategorybut);

    for (size_t i = 0; i < m_CategoryButton.size(); ++i)
    {
        m_CategoryButton[i]->Bind_EventFunction([&](_uInt iIndex) {SelectCategoryEvent(iIndex); });
    }

    return S_OK;
}

void CCreateMenu::SelectCategoryEvent(_uInt iIndex)
{
    for (auto iter : m_CategoryButton)
        iter->SetSelect(false);

    switch (TECH_TYPE(iIndex))
    {
    case Client::TECH_TYPE::ITEM:
        m_CategoryButton[iIndex]->SetSelect(true);
        break;
    case Client::TECH_TYPE::ARCHITECTURE:
        m_CategoryButton[iIndex]->SetSelect(true);
        break;
    }

    //여기서 세팅까지 하기
    m_pCharacterView->SetViewTechType(TECH_TYPE(iIndex));
}

CCreateMenu* CCreateMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCreateMenu* pCreateMenu = new CCreateMenu(pDevice, pContext);
    if (FAILED(pCreateMenu->Initalize_Prototype()))
    {
        Safe_Release(pCreateMenu);
        MSG_BOX("CREATE FAIL : CREATE MENU");
    }
    return pCreateMenu;
}

CGameObject* CCreateMenu::Clone(void* pArg)
{
    CCreateMenu* pCreateMenu = new CCreateMenu(*this);
    if (FAILED(pCreateMenu->Initialize(pArg)))
    {
        Safe_Release(pCreateMenu);
        MSG_BOX("CLONE FAIL : CREATE MENU");
    }
    return pCreateMenu;
}

void CCreateMenu::Free()
{
    __super::Free();

    for (auto iter : m_CategoryButton)
        Safe_Release(iter);

    Safe_Release(m_pCharacterView);
}
