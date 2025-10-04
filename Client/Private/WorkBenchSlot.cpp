#include "WorkBenchSlot.h"

#include "GameInstance.h"
#include "ItemSlotIcon.h"
#include "TechListSlotFont.h"

CWorkBenchSlot::CWorkBenchSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUserInterface(pDevice, pContext)
{
}

CWorkBenchSlot::CWorkBenchSlot(const CWorkBenchSlot& rhs) :
	CUserInterface(rhs)
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
	__super::Update(fDeletaTime);

	for (auto pChild : m_pChildList)
		pChild->Update(fDeletaTime);
}

void CWorkBenchSlot::Late_Update(_float fDeletaTime)
{
	for (auto pChild : m_pChildList)
		pChild->Late_Update(fDeletaTime);
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CWorkBenchSlot::Render()
{
	return S_OK;
}

void CWorkBenchSlot::MouseHoverEnter()
{
}

void CWorkBenchSlot::MouseHovering()
{
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
	m_pItemIcon->SetZOrder(static_cast<CUserInterface*>(m_pParent)->GetZOrder() + 1);
	if (FAILED(m_pItemIcon->Initialize(&ItemSlotDesc)))
		return E_FAIL;

	ADD_Child(m_pItemIcon);
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

}
