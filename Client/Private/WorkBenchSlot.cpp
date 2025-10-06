#include "WorkBenchSlot.h"

#include "GameInstance.h"
#include "ItemManager.h"

#include "GamePlayHUD.h"
#include "Level.h"
#include "CreateToolTipUI.h"

#include "ItemSlotIcon.h"
#include "TechListSlotFont.h"

CWorkBenchSlot::CWorkBenchSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CWorkBenchSlot::CWorkBenchSlot(const CWorkBenchSlot& rhs) :
	CBackGround(rhs)
{
}

HRESULT CWorkBenchSlot::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWorkBenchSlot::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_bIsMouseEvent = true;
	return S_OK;
}

void CWorkBenchSlot::Update(_float fDeletaTime)
{
	if (VISIBILITY::HIDDEN == m_eVisible)
		return;

	__super::Update(fDeletaTime);

	for (auto pChild : m_pChildList)
		pChild->Update(fDeletaTime);
}

void CWorkBenchSlot::Late_Update(_float fDeletaTime)
{
	if (VISIBILITY::HIDDEN == m_eVisible)
		return;

	for (auto pChild : m_pChildList)
		pChild->Late_Update(fDeletaTime);
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CWorkBenchSlot::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(1);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

void CWorkBenchSlot::SetItem(_uInt iItemID)
{
	m_iItemID = iItemID;
	m_pItemIcon->SetTexture(CItemManager::GetInstance()->GetItemTexture(CItemManager::ITEM_TEXTURE_TYPE::INVEN, iItemID));
}

void CWorkBenchSlot::MouseHoverEnter()
{
}

void CWorkBenchSlot::MouseHovering()
{
	auto pGamePlayHUD = static_cast<CGamePlayHUD *>(m_pGameInstance->GetCurrentHUD());
	if (pGamePlayHUD)
	{
		auto previewUI = pGamePlayHUD->GetPreViewUserInterface(1);
		if (nullptr == previewUI)
			return;

		auto pCreateTooltip = static_cast<CCreateToolTipUI*>(previewUI);
		pCreateTooltip->SettingToolTipUI(m_iItemID, {200.f, 640.f, 0.f});
		pCreateTooltip->SetVisibility(VISIBILITY::VISIBLE);
	}
}

void CWorkBenchSlot::MouseHoverExit()
{

}

void CWorkBenchSlot::MouseButtonDwon()
{
}

void CWorkBenchSlot::MouseButtonPressed()
{
}

void CWorkBenchSlot::MouseButtonUp()
{
}

HRESULT CWorkBenchSlot::ADD_Components()
{
	_float3 vScale = m_pTransformCom->GetScale();
	CItemSlotIcon::ITEM_SLOT_ICON_DESC ItemSlotDesc = {};
	ItemSlotDesc.pParentTransform = m_pTransformCom;

	ItemSlotDesc.vScale = { vScale.x * 0.7f, vScale.y * 0.7f, 0.f };
	ItemSlotDesc.vPosition = { 0.f,  ItemSlotDesc.vScale.y * 0.5f, 0.f };
	m_pItemIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
	m_pItemIcon->SetZOrder(m_iZOrder + 1);
	if (FAILED(m_pItemIcon->Initialize(&ItemSlotDesc)))
		return E_FAIL;
	ADD_Child(m_pItemIcon);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Slot_Base_Texture"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	return S_OK;
}

CWorkBenchSlot* CWorkBenchSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorkBenchSlot* pWorkBenchSlot = new CWorkBenchSlot(pDevice, pContext);
	if (FAILED(pWorkBenchSlot->Initalize_Prototype()))
	{
		Safe_Release(pWorkBenchSlot);
		MSG_BOX("CREATE FAIL : WORK BENCH SLOT");
	}
	return pWorkBenchSlot;
}

CUserInterface* CWorkBenchSlot::Clone(void* pArg)
{
	CWorkBenchSlot* pWorkBenchSlot = new CWorkBenchSlot(*this);
	if (FAILED(pWorkBenchSlot->Initialize(pArg)))
	{
		Safe_Release(pWorkBenchSlot);
		MSG_BOX("CLONE FAIL : WORK BENCH SLOT");
	}
	return pWorkBenchSlot;
}

void CWorkBenchSlot::Free()
{
	__super::Free();

	Safe_Release(m_pItemIcon);
}
