#include "ItemQuickSlotCount.h"

#include "GameInstance.h"

CItemQuickSlotCount::CItemQuickSlotCount(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{
}

CItemQuickSlotCount::CItemQuickSlotCount(const CItemQuickSlotCount& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CItemQuickSlotCount::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemQuickSlotCount::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_fFontPoint = { (_float)GetRectSize().left, (_float)GetRectSize().top };
}

void CItemQuickSlotCount::Update(_float fDeletaTime)
{
	
}

void CItemQuickSlotCount::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CItemQuickSlotCount::Render()
{
    if (!lstrcmp(m_szTopVeiwText, L"무한대"))
    {
        m_pFontCom->Render(m_szTopVeiwText, { 1.f, 1.f, 1.f, 1.f });
    }
    else
    {
        m_pFontCom->Render(m_szTopVeiwText, { 1.f, 1.f, 1.f, 1.f });
        m_pFontCom->Render(m_szUnderVeiwText, { 1.f, 1.f, 1.f, 1.f }, { 0.f, 10.f });
    }
    return S_OK;
}

void CItemQuickSlotCount::SetItemIndex(_Int iLoadedCount, _Int iItemTotalCOunt)
{
    if (0 > iLoadedCount)
    {
        wsprintf(m_szTopVeiwText, TEXT("무한대"));
    }
    else
    {
        wsprintf(m_szTopVeiwText, TEXT("%d"), iLoadedCount);
        wsprintf(m_szUnderVeiwText, TEXT("%d"), iItemTotalCOunt);
    }
}

HRESULT CItemQuickSlotCount::ADD_Components()
{
    CFontComponent::FONT_DESC FontDesc = {};
    FontDesc.pPoint = &m_fFontPoint;
    FontDesc.szUseFontName = TEXT("HanSanFont_16");
    FontDesc.vFontSize = { 50, 100 };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
        return E_FAIL;
}

CItemQuickSlotCount* CItemQuickSlotCount::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItemQuickSlotCount* pItemquickSlotCount = new CItemQuickSlotCount(pDevice, pContext);
    if (FAILED(pItemquickSlotCount->Initalize_Prototype()))
    {
        Safe_Release(pItemquickSlotCount);
        MSG_BOX("CREATE FAIL : ITEM QUICK SLOT COUNT");
    }

    return pItemquickSlotCount;
}

CGameObject* CItemQuickSlotCount::Clone(void* pArg)
{
    CItemQuickSlotCount* pItemquickSlotCount = new CItemQuickSlotCount(*this);
    if (FAILED(pItemquickSlotCount->Initialize(pArg)))
    {
        Safe_Release(pItemquickSlotCount);
        MSG_BOX("CLONE FAIL : ITEM QUICK SLOT COUNT");
    }

    return pItemquickSlotCount;
}

void CItemQuickSlotCount::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
