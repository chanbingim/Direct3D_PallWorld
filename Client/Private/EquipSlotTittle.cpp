#include "EquipSlotTittle.h"
#include "GameInstance.h"

CEquipSlotTittle::CEquipSlotTittle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUserInterface(pDevice, pContext)
{
}

CEquipSlotTittle::CEquipSlotTittle(const CEquipSlotTittle& rhs) :
	CUserInterface(rhs)
{
}

HRESULT CEquipSlotTittle::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEquipSlotTittle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	return S_OK;
}

void CEquipSlotTittle::Update(_float fDeletaTime)
{
}

void CEquipSlotTittle::Late_Update(_float fDeletaTime)
{
}

HRESULT CEquipSlotTittle::Render()
{
	m_pFontCom->Render(m_szTitle.c_str(), { 1.f, 1.f, 1.f, 1.f });
	return S_OK;
}

void CEquipSlotTittle::SetText(const WCHAR* szTitle)
{
	m_szTitle = szTitle;
	m_vTitleFontPos = { (_float)GetRectSize().left, (_float)GetRectSize().top };
}

HRESULT CEquipSlotTittle::ADD_Components()
{
	// 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
	CFontComponent::FONT_DESC FontDesc = {};
	m_vTitleFontPos = { (_float)GetRectSize().left, (_float)GetRectSize().top };
	FontDesc.pPoint = &m_vTitleFontPos;
	FontDesc.szUseFontName = TEXT("HanSanFont_16");
	FontDesc.vFontSize = { 50, 100 };

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
		return E_FAIL;
}

CEquipSlotTittle* CEquipSlotTittle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEquipSlotTittle* pEquipSlotTittle = new CEquipSlotTittle(pDevice, pContext);
	if (FAILED(pEquipSlotTittle->Initalize_Prototype()))
	{
		Safe_Release(pEquipSlotTittle);
		MSG_BOX("CREATE FAIL : EQUIP SLOT TITTLE");
	}
	return pEquipSlotTittle;
}

CGameObject* CEquipSlotTittle::Clone(void* pArg)
{
	CEquipSlotTittle* pEquipSlotTittle = new CEquipSlotTittle(*this);
	if (FAILED(pEquipSlotTittle->Initialize(pArg)))
	{
		Safe_Release(pEquipSlotTittle);
		MSG_BOX("CLONE FAIL : EQUIP SLOT TITTLE");
	}
	return pEquipSlotTittle;
}

void CEquipSlotTittle::Free()
{
	__super::Free();

	Safe_Release(m_pFontCom);
}
