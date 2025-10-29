#include "TechnologyMenu.h"

#include "GameInstance.h"
#include "TechnologyTitleBar.h"
#include "TechnologyView.h"
#include "TechnologyPointView.h"

CTechnologyMenu::CTechnologyMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUserInterface(pDevice, pContext)
{
}

CTechnologyMenu::CTechnologyMenu(const CTechnologyMenu& rhs) :
	CUserInterface(rhs)
{
}

HRESULT CTechnologyMenu::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTechnologyMenu::Initialize(void* pArg)
{
	m_iZOrder = 3;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;
	
	return S_OK;
}

void CTechnologyMenu::Update(_float fDeletaTime)
{
	for (auto pChild : m_pChildList)
		pChild->Update(fDeletaTime);
}

void CTechnologyMenu::Late_Update(_float fDeletaTime)
{
	for (auto pChild : m_pChildList)
		pChild->Late_Update(fDeletaTime);
}

HRESULT CTechnologyMenu::Render()
{
	return S_OK;
}

HRESULT CTechnologyMenu::ADD_Childs()
{
	_float3 vParentScale = m_pTransformCom->GetScale();
	CGameObject::GAMEOBJECT_DESC pObejctDesc = {};
	pObejctDesc.pParent = this;
	
	pObejctDesc.vScale = { vParentScale.x * 0.8f, vParentScale.y * 0.9f, 0.f};
	_float vViewHalfSize = pObejctDesc.vScale.y * 0.5f;
	pObejctDesc.vPosition = { -vParentScale.x * 0.5f + pObejctDesc.vScale.x * 0.5f, 0.f, 0.f};
	m_pTechnologyView = CTechnologyView::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(m_pTechnologyView->Initialize(&pObejctDesc)))
		return E_FAIL;

	pObejctDesc.vScale = { vParentScale.x * 0.8f, vParentScale.y * 0.05f, 0.f };
	pObejctDesc.vPosition = { -vParentScale.x * 0.5f + pObejctDesc.vScale.x * 0.5f,
							  -vViewHalfSize - (pObejctDesc.vScale.y * 0.5f), 0.f};
	m_pTechNologyTitleBar = CTechnologyTitleBar::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(m_pTechNologyTitleBar->Initialize(&pObejctDesc)))
		return E_FAIL;

	pObejctDesc.vScale = { vParentScale.x * 0.8f, vParentScale.y * 0.05f, 0.f };
	pObejctDesc.vPosition = { -vParentScale.x * 0.5f + pObejctDesc.vScale.x * 0.5f,
							  vViewHalfSize + (pObejctDesc.vScale.y * 0.5f), 0.f };
	m_pTechNologyPointView = CTechnologyPointView::Create(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(m_pTechNologyPointView->Initialize(&pObejctDesc)))
		return E_FAIL;

	m_pTechNologyTitleBar->SetZOrder(m_iZOrder + 1);
	m_pTechnologyView->SetZOrder(m_iZOrder + 1);
	m_pTechNologyPointView->SetZOrder(m_iZOrder + 1);

	ADD_Child(m_pTechNologyTitleBar);
	ADD_Child(m_pTechnologyView);
	ADD_Child(m_pTechNologyPointView);

	return S_OK;
}

CTechnologyMenu* CTechnologyMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTechnologyMenu* pCreateMenu = new CTechnologyMenu(pDevice, pContext);
	if (FAILED(pCreateMenu->Initalize_Prototype()))
	{
		Safe_Release(pCreateMenu);
		MSG_BOX("CREATE FAIL : CREATE MENU");
	}
	return pCreateMenu;
}

CGameObject* CTechnologyMenu::Clone(void* pArg)
{
	CTechnologyMenu* pCreateMenu = new CTechnologyMenu(*this);
	if (FAILED(pCreateMenu->Initialize(pArg)))
	{
		Safe_Release(pCreateMenu);
		MSG_BOX("CLONE FAIL : CREATE MENU");
	}
	return pCreateMenu;
}

void CTechnologyMenu::Free()
{
	__super::Free();

	Safe_Release(m_pTechNologyTitleBar);
	Safe_Release(m_pTechnologyView);
	Safe_Release(m_pTechNologyPointView);
}
