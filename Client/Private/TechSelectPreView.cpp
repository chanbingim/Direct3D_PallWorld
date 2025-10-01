#include "TechSelectPreView.h"

#include "GameInstance.h"
#include "ItemManager.h"

#include "PlayerManager.h"
#include "ItemSlotIcon.h"
#include "TechPreViewFont.h"

CTechSelectPreView::CTechSelectPreView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext),
    m_pItemManager(CItemManager::GetInstance())
{
    Safe_AddRef(m_pItemManager);
}

CTechSelectPreView::CTechSelectPreView(const CTechSelectPreView& rhs) :
    CBackGround(rhs),
    m_pItemManager(CItemManager::GetInstance())
{
    Safe_AddRef(m_pItemManager);
}

HRESULT CTechSelectPreView::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTechSelectPreView::Initialize(void* pArg)
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

void CTechSelectPreView::Update(_float fDeletaTime)
{
    if (m_pItemInfo)
    {
        m_pPreViewIcon->SetTexture(m_pItemManager->GetItemTexture(CItemManager::ITEM_TEXTURE_TYPE::INVEN, m_pItemInfo->iItemNum));
        m_pPreViewFont->SetText(m_pItemInfo->szItemName);
    }
    else
        m_pPreViewFont->SetText(TEXT(""));

    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CTechSelectPreView::Late_Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechSelectPreView::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

void CTechSelectPreView::SetViewItemInfo(const ITEM_DESC* ItemInfo)
{
    m_pItemInfo = ItemInfo;
}

HRESULT CTechSelectPreView::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Crate_InCircle"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CTechSelectPreView::ADD_Childs()
{
    _float3 vScale = m_pTransformCom->GetScale();
    CItemSlotIcon::ITEM_SLOT_ICON_DESC pItemSlotDesc = {};
    pItemSlotDesc.pParentTransform = m_pTransformCom;

    pItemSlotDesc.vScale = { vScale.x * 0.8f, vScale.y * 0.7f, 0.f};
    pItemSlotDesc.vPosition = { 0.f, -pItemSlotDesc.vScale.y * 0.7f, 0.f };
    m_pPreViewIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pPreViewIcon->SetZOrder(m_iZOrder + 1);
    if (FAILED(m_pPreViewIcon->Initialize(&pItemSlotDesc)))
        return E_FAIL;

    CGameObject::GAMEOBJECT_DESC pObejctDesc = {};
    pObejctDesc.pParent = this;
    pObejctDesc.vScale = { vScale.x * 0.8f, vScale.y * 0.2f, 0.f};
    pObejctDesc.vPosition = {0.f , pItemSlotDesc.vScale.y * 0.45f, 0.f};
    m_pPreViewFont = CTechPreViewFont::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pPreViewFont->SetZOrder(m_iZOrder + 2);
    if (FAILED(m_pPreViewFont->Initialize(&pObejctDesc)))
        return E_FAIL;


    ADD_Child(m_pPreViewIcon);
    ADD_Child(m_pPreViewFont);
    return S_OK;
}

CTechSelectPreView* CTechSelectPreView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTechSelectPreView* pTechSelectPreview = new CTechSelectPreView(pDevice, pContext);
    if (FAILED(pTechSelectPreview->Initalize_Prototype()))
    {
        Safe_Release(pTechSelectPreview);
        MSG_BOX("CREATE FAIL : TECH SELECT PREVIEW");
    }
    return pTechSelectPreview;
}

CUserInterface* CTechSelectPreView::Clone(void* pArg)
{
    CTechSelectPreView* pTechSelectPreview = new CTechSelectPreView(*this);
    if (FAILED(pTechSelectPreview->Initialize(pArg)))
    {
        Safe_Release(pTechSelectPreview);
        MSG_BOX("CLONE FAIL : TECH SELECT PREVIEW");
    }
    return pTechSelectPreview;
}

void CTechSelectPreView::Free()
{
    __super::Free();
    Safe_Release(m_pPreViewIcon);
    Safe_Release(m_pPreViewFont);
    Safe_Release(m_pItemManager);
}
