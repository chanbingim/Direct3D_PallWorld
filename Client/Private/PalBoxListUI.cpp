#include "PalBoxListUI.h"

#include "GameInstance.h"

#include "PalBoxSlot.h"
#include "TitleUI.h"

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
    m_pTitleUI->Update(fDeletaTime);

    for (auto pSlot : m_Slots)
        pSlot->Update(fDeletaTime);
}

void CPalBoxListUI::Late_Update(_float fDeletaTime)
{
    m_pTitleUI->Late_Update(fDeletaTime);
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
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Slot_Base_Texture"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalBoxListUI::ADD_Childs()
{
    _float3 vScale = m_pTransformCom->GetScale();
    CTitleUI::TITLE_UI_DESC TitleDesc = {};
    TitleDesc.pParent = this;
    TitleDesc.vScale = { vScale.x, 20.f, 0.f };
    TitleDesc.vPosition = { 0.f, -vScale.y * 0.5f + TitleDesc.vScale.y, 0.f };
    TitleDesc.szTitleName = TEXT("¹Ú½º");

    m_pTitleUI = static_cast<CTitleUI*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Base_Title_UI"), &TitleDesc));
    m_pTitleUI->SetZOrder(m_iZOrder + 1);
    ADD_Child(m_pTitleUI);

    CPalBoxSlot::PAL_BOX_DESC PalBoxSlotDesc = {};
    PalBoxSlotDesc.pParent = this;
    PalBoxSlotDesc.vScale = { 50.f, 50.f, 0.f };
    PalBoxSlotDesc.ePalSlotType = CPalBoxSlot::PAL_SLOT_TYPE::BOX;

    m_SlotCount = {6, 5};
    _float3 vParentScale = m_pTransformCom->GetScale();
    _float3 vStartPos = { -vScale.x * 0.5f + 50 , -vScale.y * 0.5f + TitleDesc.vScale.y + 50, 0.f };
    for (_uInt i = 0; i < m_SlotCount.y; ++i)
    {
        for (_uInt j = 0; j < m_SlotCount.x; ++j)
        {
            PalBoxSlotDesc.vPosition.x = vStartPos.x + (PalBoxSlotDesc.vScale.x + 10) * j;
            PalBoxSlotDesc.vPosition.y = vStartPos.y + (PalBoxSlotDesc.vScale.y + 10) * i;
            CPalBoxSlot* pPalBoxSlot = CPalBoxSlot::Create(m_pGraphic_Device, m_pDeviceContext);
            if (nullptr == pPalBoxSlot)
                return E_FAIL;

            pPalBoxSlot->SetZOrder(m_iZOrder + 1);
            if (FAILED(pPalBoxSlot->Initialize(&PalBoxSlotDesc)))
                return E_FAIL;

            pPalBoxSlot->SetPalSlotNumber(_uInt(i * m_SlotCount.x + j));
            m_Slots.push_back(pPalBoxSlot);
        }
    }

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

    Safe_Release(m_pTitleUI);
    for (auto pSlot : m_Slots)
        Safe_Release(pSlot);

    m_Slots.clear();
}
