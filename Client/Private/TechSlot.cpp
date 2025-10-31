#include "TechSlot.h"

#include "GameInstance.h"

#include "TechSlotNameBar.h"
#include "ItemSlotIcon.h"

#include "TechManager.h"
#include "TechSlotTitleBar.h"
#include "ItemManager.h"

#include "GamePlayHUD.h"
#include "SelectUI.h"
#include "Level.h"

CTechSlot::CTechSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CSlotBase(pDevice, pContext, SLOT_TYPE::TECH),
	m_pTechManager(CTechManager::GetInstance())
{
	Safe_AddRef(m_pTechManager);
}

CTechSlot::CTechSlot(const CTechSlot& rhs) :
	CSlotBase(rhs),
	m_pTechManager(CTechManager::GetInstance())
{
	Safe_AddRef(m_pTechManager);
}

HRESULT CTechSlot::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTechSlot::Initialize(void* pArg)
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

void CTechSlot::Update(_float fDeletaTime)
{
	if (VISIBILITY::HIDDEN == m_eVisible)
		return;

	UpdateRectSize();
	for (auto pChild : m_pChildList)
		pChild->Update(fDeletaTime);

	__super::Update(fDeletaTime);
}

void CTechSlot::Late_Update(_float fDeletaTime)
{
	if (VISIBILITY::HIDDEN == m_eVisible)
		return;

	for (auto pChild : m_pChildList)
		pChild->Late_Update(fDeletaTime);

	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechSlot::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(2);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

void CTechSlot::SetSlotItem(const TECH_ITEM_DESC& TehcItem)
{
	m_TechItem = &TehcItem;
	m_pItemDesc = m_pItemManager->GetItemInfo(m_TechItem->ReturnItemID);

	m_pSlotIcon->SetTexture(m_pItemManager->GetItemTexture(CItemManager::ITEM_TEXTURE_TYPE::INVEN, m_TechItem->ReturnItemID));
	m_pSlotTitleBar->SetTehcItemTypeName(GetTechTypeName(m_TechItem->TechType));
	m_pSlotItemName->SetTehcItemName(m_pItemDesc->szItemName);
}

void CTechSlot::UseSlot(void* pArg)
{
}

void CTechSlot::MouseHoverEnter()
{
}

void CTechSlot::MouseHovering()
{
}

void CTechSlot::MouseHoverExit()
{
}

void CTechSlot::MouseButtonDwon()
{
	auto GamePlayHUD = static_cast<CGamePlayHUD*>(m_pGameInstance->GetCurrentLevel()->GetHUD());
	auto SelectUI = GamePlayHUD->GetSelectUI();

	if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 1))
	{
		GamePlayHUD->SetVisibleSelectUI(VISIBILITY::VISIBLE);
		SelectUI->SetText(0, TEXT("배우기"));
		SelectUI->SetText(1, TEXT("취소"));
		SelectUI->SetPosition({ (_float)GetRectSize().right,
							   (_float)GetRectSize().bottom, 0.f });

		SelectUI->BindEvent(1, [&]() { CancelEvent(); });
		SelectUI->BindEvent(0, [&]() {  LearnEvent(); });
	}
	else
	{
		if(false == PtInRect(&SelectUI->GetRectSize(), m_pGameInstance->GetMousePoint()))
			GamePlayHUD->SetVisibleSelectUI(VISIBILITY::HIDDEN);
		else
		{

		}
	}
}

void CTechSlot::MouseButtonPressed()
{
}

void CTechSlot::MouseButtonUp()
{
}

HRESULT CTechSlot::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Slot_Base_Texture"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

const WCHAR* CTechSlot::GetTechTypeName(TECH_TYPE eTechType)
{
	switch (eTechType)
	{
	case TECH_TYPE::PELL:
		return TEXT("펠");
	case TECH_TYPE::ITEM:
		return TEXT("아이템");
	case TECH_TYPE::ARCHITECTURE:
		return TEXT("건축물");
	}

	return TEXT("");
}

void CTechSlot::LearnEvent()
{
	m_pTechManager->LernTechObject(m_TechItem->TechTypeID);
	auto GamePlayHUD = static_cast<CGamePlayHUD*>(m_pGameInstance->GetCurrentLevel()->GetHUD());
	GamePlayHUD->SetVisibleSelectUI(VISIBILITY::HIDDEN);
}

void CTechSlot::CancelEvent()
{
	auto GamePlayHUD = static_cast<CGamePlayHUD*>(m_pGameInstance->GetCurrentLevel()->GetHUD());
	GamePlayHUD->SetVisibleSelectUI(VISIBILITY::HIDDEN);
}

HRESULT CTechSlot::ADD_Childs()
{
	CItemSlotIcon::ITEM_SLOT_ICON_DESC pItemSlottDesc = {};
	_float3		vParentScale = m_pTransformCom->GetScale();
	_float		vIconHalfSzieY = vParentScale.y * 0.5f;

	pItemSlottDesc.pParent = this;
	pItemSlottDesc.vScale = { vParentScale.x, vParentScale.y * 0.5f, 0.f };
	pItemSlottDesc.vPosition = { 0.f, 0.f, 0.f };
	pItemSlottDesc.pParentTransform = m_pTransformCom;
	m_pSlotIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(m_pSlotIcon->Initialize(&pItemSlottDesc)))
		return E_FAIL;

	CGameObject::GAMEOBJECT_DESC pObjectDesc = {};
	pObjectDesc.pParent = this;
	pObjectDesc.vScale = { vParentScale.x, vParentScale.y * 0.2f, 0.f};
	pObjectDesc.vPosition = {0.f, -vIconHalfSzieY + (pObjectDesc.vScale.y * 0.5f) , 0.f};
	m_pSlotTitleBar = CTechSlotTitleBar::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(m_pSlotTitleBar->Initialize(&pObjectDesc)))
		return E_FAIL;

	pObjectDesc.vScale = { vParentScale.x, vParentScale.y * 0.2f, 0.f };
	pObjectDesc.vPosition = { 0.f, vIconHalfSzieY - (pObjectDesc.vScale.y * 0.5f) , 0.f };
	m_pSlotItemName = CTechSlotNameBar::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(m_pSlotItemName->Initialize(&pObjectDesc)))
		return E_FAIL;

	m_pSlotIcon->SetZOrder(m_iZOrder + 1);
	m_pSlotTitleBar->SetZOrder(m_iZOrder + 1);
	m_pSlotItemName->SetZOrder(m_iZOrder + 1);

	ADD_Child(m_pSlotIcon);
	ADD_Child(m_pSlotTitleBar);
	ADD_Child(m_pSlotItemName);

	return S_OK;
}

CTechSlot* CTechSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTechSlot* pTechSlot = new CTechSlot(pDevice, pContext);
	if (FAILED(pTechSlot->Initalize_Prototype()))
	{
		Safe_Release(pTechSlot);
		MSG_BOX("CREATE FAIL : TECH SLOT");
	}
	return pTechSlot;
}

CUserInterface* CTechSlot::Clone(void* pArg)
{
	CTechSlot* pTechSlot = new CTechSlot(*this);
	if (FAILED(pTechSlot->Initialize(pArg)))
	{
		Safe_Release(pTechSlot);
		MSG_BOX("CLONE FAIL : TECH SLOT");
	}
	return pTechSlot;
}

void CTechSlot::Free()
{
	__super::Free();

	Safe_Release(m_pSlotTitleBar);
	Safe_Release(m_pSlotIcon);
	Safe_Release(m_pSlotItemName);
	Safe_Release(m_pTechManager);
}
