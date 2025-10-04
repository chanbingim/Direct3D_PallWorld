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

HRESULT CWorkBenchSlotView::ADD_Components()
{
	CWorkBenchSlot* pWorkBenchSlot = nullptr;
	CWorkBenchSlot::GAMEOBJECT_DESC pSlotDesc = {};
	pSlotDesc.pParent = this;
	pSlotDesc.vScale = { 30.f, 30.f, 0.f };

	for (auto i = 0; i < m_iItemCount; ++i)
	{
		for (auto j = 0; j < m_iItemCount; ++j)
		{
			pSlotDesc.vPosition = {};
			pWorkBenchSlot = CWorkBenchSlot::Create(m_pGraphic_Device, m_pDeviceContext);
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
	if (FAILED(pWorkBenchSlotView->Initalize_Prototype()))
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
