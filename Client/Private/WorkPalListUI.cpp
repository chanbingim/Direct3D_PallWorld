#include "WorkPalListUI.h"

#include "GameInstance.h"

#include "PalBoxSlot.h"
#include "TitleUI.h"

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
	m_pTitleUI->Update(fDeletaTime);

	for (auto pSlot : m_Slots)
		pSlot->Update(fDeletaTime);
}

void CWorkPalListUI::Late_Update(_float fDeletaTime)
{
	m_pTitleUI->Late_Update(fDeletaTime);
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Slot_Base_Texture"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWorkPalListUI::ADD_Childs()
{
	_float3 vScale = m_pTransformCom->GetScale();
	CTitleUI::TITLE_UI_DESC TitleDesc = {};
	TitleDesc.pParent = this;
	TitleDesc.vScale = { vScale.x, 20.f, 0.f };
	TitleDesc.vPosition = { 0.f, -vScale.y + TitleDesc.vScale.y * 0.5f, 0.f };
	TitleDesc.szTitleName = TEXT("¹Ú½º");

	m_pTitleUI = static_cast<CTitleUI*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Base_Title_UI"), &TitleDesc));
	m_pTitleUI->SetZOrder(m_iZOrder + 1);
	ADD_Child(m_pTitleUI);

	CPalBoxSlot::PAL_BOX_DESC PalBoxSlotDesc = {};
	PalBoxSlotDesc.pParent = this;
	PalBoxSlotDesc.vScale = { 50.f, 50.f, 0.f };
	PalBoxSlotDesc.ePalSlotType = CPalBoxSlot::PAL_SLOT_TYPE::WORK;

	m_SlotCount = { 5, 2 };
	_float3 vParentScale = m_pTransformCom->GetScale();
	_float3 vStartPos = { (_float)m_UISize.left + 60 , (_float)m_UISize.top + 110, 0.f };
	for (_uInt i = 0; i < m_SlotCount.y; ++i)
	{
		for (_uInt j = 0; j < m_SlotCount.x; ++j)
		{
			PalBoxSlotDesc.vPosition.x = vStartPos.x + (PalBoxSlotDesc.vScale.x + 10) * j;
			CPalBoxSlot* pPalBoxSlot = CPalBoxSlot::Create(m_pGraphic_Device, m_pDeviceContext);
			if (nullptr == pPalBoxSlot)
				return E_FAIL;

			pPalBoxSlot->SetZOrder(m_iZOrder + 1);
			if (FAILED(pPalBoxSlot->Initialize(&PalBoxSlotDesc)))
				return E_FAIL;

			pPalBoxSlot->SetPalSlotNumber(_uInt(i * m_SlotCount.x + j));
			m_Slots.push_back(pPalBoxSlot);
		}
	}

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

	Safe_Release(m_pTitleUI);
	for (auto pSlot : m_Slots)
		Safe_Release(pSlot);

	m_Slots.clear();
}
