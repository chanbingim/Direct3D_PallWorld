#include "PalBoxListUI.h"

#include "GameInstance.h"
#include "PalBoxSlot.h"

CPalBoxListUI::CPalBoxListUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CPalBoxListUI::CPalBoxListUI(const CPalBoxListUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CPalBoxListUI::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalBoxListUI::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CPalBoxListUI::Update(_float fDeletaTime)
{
    for (auto pSlot : m_Slots)
        pSlot->Update(fDeletaTime);
}

void CPalBoxListUI::Late_Update(_float fDeletaTime)
{
    for (auto pSlot : m_Slots)
        pSlot->Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CPalBoxListUI::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CPalBoxListUI::ADD_Components()
{
    return S_OK;
}

HRESULT CPalBoxListUI::ADD_Childs()
{
    return S_OK;
}

CPalBoxListUI* CPalBoxListUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPalBoxListUI* pPalBoxListUI = new CPalBoxListUI(pDevice, pContext);
    if (FAILED(pPalBoxListUI->Initalize_Prototype()))
    {
        Safe_Release(pPalBoxListUI);
        MSG_BOX("CREATE FAIL : PAL BOX LIST UI");
    }
    return pPalBoxListUI;
}

CGameObject* CPalBoxListUI::Clone(void* pArg)
{
    CPalBoxListUI* pPalBoxListUI = new CPalBoxListUI(*this);
    if (FAILED(pPalBoxListUI->Initialize(pArg)))
    {
        Safe_Release(pPalBoxListUI);
        MSG_BOX("CLONE FAIL : PAL BOX LIST UI");
    }
    return pPalBoxListUI;
}

void CPalBoxListUI::Free()
{
    __super::Free();

    for (auto pSlot : m_Slots)
        Safe_Release(pSlot);

    m_Slots.clear();
}
