#include "InGameMenu.h"

#include "GameInstance.h"
#include "Category.h"
#include "GameOption.h"
#include "CharacterView.h"

CInGameMenu::CInGameMenu(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CBackGround(pGraphic_Device, pDeviceContext)
{
}

CInGameMenu::CInGameMenu(const CInGameMenu& rhs) :
    CBackGround(rhs)
{
}

HRESULT CInGameMenu::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CInGameMenu::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    m_iZOrder = 2;

    return S_OK;
}

void CInGameMenu::Update(_float fDeletaTime)
{
    if (!m_bIsActive)
        return;

    for (auto& iter : m_pChildList)
        iter->Update(fDeletaTime);

    if(m_pSelectWidget)
        m_pSelectWidget->Update(fDeletaTime);
}

void CInGameMenu::Late_Update(_float fDeletaTime)
{
    if (!m_bIsActive)
        return;

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);

    for (auto& iter : m_pChildList)
        iter->Late_Update(fDeletaTime);

    if (m_pSelectWidget)
        m_pSelectWidget->Late_Update(fDeletaTime);
}

HRESULT CInGameMenu::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

void CInGameMenu::SetActive(_bool flag)
{
    m_bIsActive = flag;
  
    //버튼 애니메이션 초기화후 버튼 애니메이션 재생
    /*for (auto& iter : m_CategoryButton)
        iter->SetActive(flag);*/
}

_bool CInGameMenu::IsActive()
{
    return m_bIsActive;
}

HRESULT CInGameMenu::ADD_Childs()
{
    if (FAILED(ADD_CategoryButton()))
        return E_FAIL;

    if (FAILED(ADD_Widgets()))
        return E_FAIL;

    return S_OK;
}

HRESULT CInGameMenu::ADD_CategoryButton()
{
    CCategory* pCategorybut = nullptr;

    // 버튼 만들기
    CCategory::CATEGROY_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 150.f, 30.f, 0.f };

    float ButHalfX = Desc.vScale.x * 0.5f + 3.f;
    float CenterX = m_pTransformCom->GetScale().x * 0.5f - ButHalfX;

    _float fButtonPosY = -325;
    /* CharacterInfo Button */
    Desc.Type = 0;
    Desc.vPosition = { -CenterX, fButtonPosY, 0.f };
    pCategorybut = CCategory::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(pCategorybut->Initialize(&Desc)))
        return E_FAIL;
    m_CategoryButton.push_back(pCategorybut);
    ADD_Child(pCategorybut);

    /* Option Button */
    Desc.Type = 1;
    Desc.vPosition = { -(CenterX - (ButHalfX * 2.f)), fButtonPosY, 0.f };
    pCategorybut = CCategory::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(pCategorybut->Initialize(&Desc)))
        return E_FAIL;
    m_CategoryButton.push_back(pCategorybut);
    ADD_Child(pCategorybut);

    for (size_t i = 0; i < m_CategoryButton.size(); ++i)
    {
        m_CategoryButton[i]->Bind_ClickEvent([&](_uInt iIndex) {SelectCategoryEvent(iIndex); });
    }

    return S_OK;
}

HRESULT CInGameMenu::ADD_Widgets()
{
    CBackGround::GAMEOBJECT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CBackGround::GAMEOBJECT_DESC));

    Desc.pParent = this;
    Desc.vScale = m_pTransformCom->GetScale();
    m_pGameOptionUI = CGameOption::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(m_pGameOptionUI->Initialize(&Desc)))
        return E_FAIL;

    m_pCharacterView = CCharacterView::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(m_pCharacterView->Initialize(&Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CInGameMenu::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

void CInGameMenu::SelectCategoryEvent(_uInt iIndex)
{
    for (size_t i = 0; i < m_CategoryButton.size(); ++i)
        m_CategoryButton[i]->SetActive(false);

    m_CategoryButton[iIndex]->SetActive(true);
    if (m_pSelectWidget)
        m_pSelectWidget->SetVisibility(VISIBILITY::HIDDEN);

    switch (iIndex)
    {
    case 0 :
        m_pSelectWidget = m_pCharacterView;
        m_pCharacterView->SetVisibility(VISIBILITY::VISIBLE);
        break;
    case 1:
        m_pSelectWidget = m_pGameOptionUI;
        m_pGameOptionUI->SetVisibility(VISIBILITY::VISIBLE);
        break;
    }
}

CInGameMenu* CInGameMenu::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CInGameMenu* pInGameMenu = new CInGameMenu(pGraphic_Device, pDeviceContext);
    if (FAILED(pInGameMenu->Initalize_Prototype()))
    {
        Safe_Release(pInGameMenu);
        MSG_BOX("CREATE FAIL : IN GAME MENU");
    }
    return pInGameMenu;
}

CGameObject* CInGameMenu::Clone(void* pArg)
{
    CInGameMenu* pInGameMenu = new CInGameMenu(*this);
    if (FAILED(pInGameMenu->Initialize(pArg)))
    {
        Safe_Release(pInGameMenu);
        MSG_BOX("CLONE FAIL : IN GAME MENU");
    }
    return pInGameMenu;
}

void CInGameMenu::Free()
{
    __super::Free();

    for (auto& iter : m_CategoryButton)
        Safe_Release(iter);

    Safe_Release(m_pGameOptionUI);
    Safe_Release(m_pCharacterView);
}
