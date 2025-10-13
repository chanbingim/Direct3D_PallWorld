#include "PalInvenListUI.h"

#include "GameInstance.h"
#include "PalBoxSlot.h"

CPalInvenListUI::CPalInvenListUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CPalInvenListUI::CPalInvenListUI(const CPalInvenListUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CPalInvenListUI::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalInvenListUI::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CPalInvenListUI::Update(_float fDeletaTime)
{
    for (auto pSlot : m_Slots)
        pSlot->Update(fDeletaTime);


}

void CPalInvenListUI::Late_Update(_float fDeletaTime)
{
    for (auto pSlot : m_Slots)
        pSlot->Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CPalInvenListUI::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CPalInvenListUI::ADD_Components()
{
    


    return S_OK;
}

HRESULT CPalInvenListUI::ADD_Childs()
{

    return S_OK;
}

CPalInvenListUI* CPalInvenListUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPalInvenListUI* pPalInvenList = new CPalInvenListUI(pDevice, pContext);
    if (FAILED(pPalInvenList->Initalize_Prototype()))
    {
        Safe_Release(pPalInvenList);
        MSG_BOX("CREATE FAIL : Pal Inven List");
    }
    return pPalInvenList;
}

CGameObject* CPalInvenListUI::Clone(void* pArg)
{
    CPalInvenListUI* pPalInvenList = new CPalInvenListUI(*this);
    if (FAILED(pPalInvenList->Initialize(pArg)))
    {
        Safe_Release(pPalInvenList);
        MSG_BOX("CLONE FAIL : Pal Inven List");
    }
    return pPalInvenList;
}

void CPalInvenListUI::Free()
{
    __super::Free();

    for (auto pSlot : m_Slots)
        Safe_Release(pSlot);

    m_Slots.clear();
}
