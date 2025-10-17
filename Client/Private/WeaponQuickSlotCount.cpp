#include "WeaponQuickSlotCount.h"

#include "GameInstance.h"

CWeaponQuickSlotCount::CWeaponQuickSlotCount(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUserInterface(pDevice, pContext)
{
}

CWeaponQuickSlotCount::CWeaponQuickSlotCount(const CWeaponQuickSlotCount& rhs) :
	CUserInterface(rhs)
{
}

HRESULT CWeaponQuickSlotCount::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponQuickSlotCount::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	m_iZOrder = 2;
	m_fFontPoint = { (_float)GetRectSize().left, (_float)GetRectSize().top };
	return S_OK;
}

void CWeaponQuickSlotCount::Update(_float fDeletaTime)
{

}

void CWeaponQuickSlotCount::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CWeaponQuickSlotCount::Render()
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

void CWeaponQuickSlotCount::SetItemIndex(_Int iLoadedCount, _Int iItemTotalCOunt)
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

HRESULT CWeaponQuickSlotCount::ADD_Components()
{
	CFontComponent::FONT_DESC FontDesc = {};
	FontDesc.pPoint = &m_fFontPoint;
	FontDesc.szUseFontName = TEXT("HanSanFont_16");
	FontDesc.vFontSize = {50, 100};

	if(FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
		return E_FAIL;

	return S_OK;
}

CWeaponQuickSlotCount* CWeaponQuickSlotCount::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeaponQuickSlotCount* pWeaponquickSlotCount = new CWeaponQuickSlotCount(pDevice, pContext);
	if (FAILED(pWeaponquickSlotCount->Initalize_Prototype()))
	{
		Safe_Release(pWeaponquickSlotCount);
		MSG_BOX("CREATE FAIL : WEAPON QUICK SLOT COUNT");
	}

	return pWeaponquickSlotCount;
}

CGameObject* CWeaponQuickSlotCount::Clone(void* pArg)
{
	CGameObject* pWeaponquickSlotCount = new CWeaponQuickSlotCount(*this);
	if (FAILED(pWeaponquickSlotCount->Initialize(pArg)))
	{
		Safe_Release(pWeaponquickSlotCount);
		MSG_BOX("CLONE FAIL : WEAPON QUICK SLOT COUNT");
	}

	return pWeaponquickSlotCount;
}

void CWeaponQuickSlotCount::Free()
{
	__super::Free();

	Safe_Release(m_pFontCom);
}
