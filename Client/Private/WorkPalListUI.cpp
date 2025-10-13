#include "WorkPalListUI.h"

#include "GameInstance.h"
#include "PalBoxSlot.h"

CWorkPalListUI::CWorkPalListUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CWorkPalListUI::CWorkPalListUI(const CWorkPalListUI& rhs) :
	CBackGround(rhs)
{
}

HRESULT CWorkPalListUI::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWorkPalListUI::Initialize(void* pArg)
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

void CWorkPalListUI::Update(_float fDeletaTime)
{
	for (auto pSlot : m_Slots)
		pSlot->Update(fDeletaTime);
}

void CWorkPalListUI::Late_Update(_float fDeletaTime)
{
	for (auto pSlot : m_Slots)
		pSlot->Late_Update(fDeletaTime);

	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CWorkPalListUI::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(0);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CWorkPalListUI::ADD_Components()
{
	return S_OK;
}

HRESULT CWorkPalListUI::ADD_Childs()
{
	return S_OK;
}

CWorkPalListUI* CWorkPalListUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorkPalListUI* pWorkPalListUI = new CWorkPalListUI(pDevice, pContext);
	if (FAILED(pWorkPalListUI->Initalize_Prototype()))
	{
		Safe_Release(pWorkPalListUI);
		MSG_BOX("CREATE FAIL : Work Pal List UI");
	}
	return pWorkPalListUI;
}

CGameObject* CWorkPalListUI::Clone(void* pArg)
{
	CWorkPalListUI* pWorkPalListUI = new CWorkPalListUI(*this);
	if (FAILED(pWorkPalListUI->Initalize_Prototype()))
	{
		Safe_Release(pWorkPalListUI);
		MSG_BOX("CLONE FAIL : Work Pal List UI");
	}
	return pWorkPalListUI;
}

void CWorkPalListUI::Free()
{
	__super::Free();

	for (auto pSlot : m_Slots)
		Safe_Release(pSlot);

	m_Slots.clear();
}
