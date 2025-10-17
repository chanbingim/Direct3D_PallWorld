#include "SelectUI.h"

#include "GameInstance.h"
#include "EventButton.h"

CSelectUI::CSelectUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CSelectUI::CSelectUI(const CSelectUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CSelectUI::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSelectUI::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    return S_OK;
}

void CSelectUI::Update(_float fDeletaTime)
{
    if (VISIBILITY::HIDDEN == m_eVisible)
        return;

    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CSelectUI::Late_Update(_float fDeletaTime)
{
    if (VISIBILITY::HIDDEN == m_eVisible)
        return;

    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CSelectUI::Render()
{
    return S_OK;
}

void CSelectUI::BindEvent(_uInt iButtonType, function<void()> bindEvent) const
{
    switch (iButtonType)
    {
    case 0 :
        m_pOKButton->Bind_Event(bindEvent);
        break;

    case 1 :
        m_pCancelButton->Bind_Event(bindEvent);
        break;
    }
}

void CSelectUI::SetText(_uInt iButtonType, const WCHAR* szText) const
{
    switch (iButtonType)
    {
    case 0:
        m_pOKButton->Set_Text(szText);
        break;

    case 1:
        m_pCancelButton->Set_Text(szText);
        break;
    }
}

void CSelectUI::SetPosition(const _float3& vPos) 
{
    SetLocation(vPos);
}

HRESULT CSelectUI::ADD_Childs()
{
    _float3 vScale = m_pTransformCom->GetScale();
    CEventButton::GAMEOBJECT_DESC pEventDesc = {};
    pEventDesc.pParent = this;
    pEventDesc.vScale = { vScale.x, vScale.y * 0.5f, 0.f };

    pEventDesc.vPosition = { 0.f, -pEventDesc.vScale.y * 0.5f, 0.f };
    m_pCancelButton = static_cast<CEventButton*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Event_Button"), &pEventDesc));
    if (nullptr == m_pCancelButton)
        return E_FAIL;

    pEventDesc.vPosition = { 0.f, pEventDesc.vScale.y * 0.5f, 0.f };
    m_pOKButton = static_cast<CEventButton*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Event_Button"), &pEventDesc));
    if (nullptr == m_pOKButton)
        return E_FAIL;

    m_pCancelButton->SetZOrder(m_iZOrder + 1);
    m_pOKButton->SetZOrder(m_iZOrder + 1);

    ADD_Child(m_pCancelButton);
    ADD_Child(m_pOKButton);
    return S_OK;
}

CSelectUI* CSelectUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSelectUI* pSelectUI = new CSelectUI(pDevice, pContext);
    if (FAILED(pSelectUI->Initalize_Prototype()))
    {
        Safe_Release(pSelectUI);
        MSG_BOX("CREATE FAIL : SELECT UI");
    }
    return pSelectUI;
}

CUserInterface* CSelectUI::Clone(void* pArg)
{
    CSelectUI* pSelectUI = new CSelectUI(*this);
    if (FAILED(pSelectUI->Initialize(pArg)))
    {
        Safe_Release(pSelectUI);
        MSG_BOX("CLONE FAIL : SELECT UI");
    }
    return pSelectUI;
}

void CSelectUI::Free()
{
    __super::Free();

    Safe_Release(m_pCancelButton);
    Safe_Release(m_pOKButton);
}
