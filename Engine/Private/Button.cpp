#include "Button.h"

#include "GameInstance.h"

CButton::CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{
}

CButton::CButton(const CButton& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CButton::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CButton::Initialize(void* pArg)
{
    return S_OK;
}

void CButton::Update(_float fDeletaTime)
{
}

HRESULT CButton::Render()
{
    return S_OK;
}

CButton* CButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CButton* pButton = new CButton(pDevice, pContext);
    if (FAILED(pButton->Initalize_Prototype()))
    {
        Safe_Release(pButton);
        MSG_BOX("CREATE FAIL : BUTTON");
    }
    return pButton;
}

CGameObject* CButton::Clone(void* pArg)
{
    CGameObject* pButton = new CButton(*this);
    if (FAILED(pButton->Initialize(pArg)))
    {
        Safe_Release(pButton);
        MSG_BOX("CLONE FAIL : BUTTON");
    }

    return pButton;
}

void CButton::Free()
{
    __super::Free();
}
