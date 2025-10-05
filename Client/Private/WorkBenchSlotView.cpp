#include "WorkBenchSlotView.h"

#include "GameInstance.h"
#include "WorkBenchSlot.h"

CWorkBenchSlotView::CWorkBenchSlotView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUserInterface(pDevice, pContext)
{
}

CWorkBenchSlotView::CWorkBenchSlotView(const CWorkBenchSlotView& rhs) :
	CUserInterface(rhs)
{
}

HRESULT CWorkBenchSlotView::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWorkBenchSlotView::Initialize(void* pArg)
{
	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	WORK_BENCH_SLOT_VIEW_DESC* pSlotViewDesc = static_cast<WORK_BENCH_SLOT_VIEW_DESC*>(pArg);
	m_iItemCount = pSlotViewDesc->iSlotCount;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	return S_OK;
}

void CWorkBenchSlotView::Update(_float fDeletaTime)
{
	for (auto pSlot : m_ItemSlot)
		pSlot->Update(fDeletaTime);
}

void CWorkBenchSlotView::Late_Update(_float fDeletaTime)
{
	for (auto pSlot : m_ItemSlot)
		pSlot->Late_Update(fDeletaTime);
}

HRESULT CWorkBenchSlotView::Render()
{



	return S_OK;
}

void CWorkBenchSlotView::SetViewItemList(const vector<_uInt>& ItemList)
{
	for (size_t i = 0; i < m_ItemSlot.size(); ++i)
	{
		if (i >= ItemList.size())
		{
			m_ItemSlot[i]->SetVisibility(VISIBILITY::HIDDEN);
		}
		else
		{
			m_ItemSlot[i]->SetItem(ItemList[i]);
			m_ItemSlot[i]->SetVisibility(VISIBILITY::VISIBLE);
		}
	}
}

HRESULT CWorkBenchSlotView::ADD_Components()
{
	_float3 vParentScale = m_pTransformCom->GetScale();
	_float	fParentZorder = static_cast<CUserInterface*>(m_pParent)->GetZOrder();

	CWorkBenchSlot* pWorkBenchSlot = nullptr;
	CWorkBenchSlot::GAMEOBJECT_DESC pSlotDesc = {};
	pSlotDesc.pParent = this;
	pSlotDesc.vScale = { vParentScale.x / m_iItemCount, vParentScale.x / m_iItemCount, 0.f };

	_float	vSlotPosX = -vParentScale.x * 0.5f + pSlotDesc.vScale.x * 0.5f;
	_float	vSlotPosY = -vParentScale.y * 0.5f + pSlotDesc.vScale.y * 0.5f;

	for (auto i = 0; i < m_iItemCount; ++i)
	{
		for (auto j = 0; j < m_iItemCount; ++j)
		{
			pSlotDesc.vPosition = {vSlotPosX + pSlotDesc.vScale.x * j, vSlotPosY + pSlotDesc.vScale.y * i , 0.f};
			pWorkBenchSlot = CWorkBenchSlot::Create(m_pGraphic_Device, m_pDeviceContext);
			pWorkBenchSlot->SetZOrder(fParentZorder + 1);
			if (FAILED(pWorkBenchSlot->Initialize(&pSlotDesc)))
				return E_FAIL;

			m_ItemSlot.push_back(pWorkBenchSlot);
		}
	}

	return S_OK;
}

CWorkBenchSlotView* CWorkBenchSlotView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorkBenchSlotView* pWorkBenchSlotView = new CWorkBenchSlotView(pDevice, pContext);
	if (FAILED(pWorkBenchSlotView->Initalize_Prototype()))
	{
		Safe_Release(pWorkBenchSlotView);
		MSG_BOX("CREATE FAIL : WORK BENCH SLOT VIEW");
	}
	return pWorkBenchSlotView;
}

CGameObject* CWorkBenchSlotView::Clone(void* pArg)
{
	CWorkBenchSlotView* pWorkBenchSlotView = new CWorkBenchSlotView(*this);
	if (FAILED(pWorkBenchSlotView->Initialize(pArg)))
	{
		Safe_Release(pWorkBenchSlotView);
		MSG_BOX("CLONE FAIL : WORK BENCH SLOT VIEW");
	}
	return pWorkBenchSlotView;
}

void CWorkBenchSlotView::Free()
{
	__super::Free();

	for (auto pSlot : m_ItemSlot)
		Safe_Release(pSlot);
}
