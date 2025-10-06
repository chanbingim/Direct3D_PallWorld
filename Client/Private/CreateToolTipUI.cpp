#include "CreateToolTipUI.h"

#include "GameInstance.h"
#include "ItemManager.h"
#include "TechManager.h"

#include "ItemSlotIcon.h"
#include "IngredientList.h"

CCreateToolTipUI::CCreateToolTipUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CCreateToolTipUI::CCreateToolTipUI(const CCreateToolTipUI& rhs) :
	CBackGround(rhs)
{
}

HRESULT CCreateToolTipUI::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCreateToolTipUI::Initialize(void* pArg)
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

void CCreateToolTipUI::Update(_float fDeletaTime)
{
	if (VISIBILITY::HIDDEN == m_eVisible)
		return;

	for (auto pChild : m_pChildList)
		pChild->Update(fDeletaTime);
}

void CCreateToolTipUI::Late_Update(_float fDeletaTime)
{
	if (VISIBILITY::HIDDEN == m_eVisible)
		return;

	for (auto pChild : m_pChildList)
		pChild->Late_Update(fDeletaTime);

	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CCreateToolTipUI::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(2);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

void CCreateToolTipUI::SettingToolTipUI(_uInt iItemID, _float3 vPosition)
{
	SetLocation(vPosition);
	m_pItemIcon->SetTexture(CItemManager::GetInstance()->GetItemTexture(CItemManager::ITEM_TEXTURE_TYPE::INVEN, iItemID));

}

HRESULT CCreateToolTipUI::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellInfo_Background"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCreateToolTipUI::ADD_Childs()
{
	_float3 vScale = m_pTransformCom->GetScale();
	_float	fParentZorder = {};
	
	if(nullptr != m_pParent)
		fParentZorder = static_cast<CUserInterface*>(m_pParent)->GetZOrder();

	CItemSlotIcon::ITEM_SLOT_ICON_DESC SlotIconDesc = {};
	SlotIconDesc.pParentTransform = m_pTransformCom;
	SlotIconDesc.vScale = { vScale.y * 0.4f, vScale.y * 0.4f, 0.f };
	SlotIconDesc.vPosition = { 0.f, -vScale.y * 0.5f + SlotIconDesc.vScale.y * 0.5f, 0.f };
	m_pItemIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
	m_pItemIcon->SetZOrder(fParentZorder + 1);
	if (FAILED(m_pItemIcon->Initialize(&SlotIconDesc)))
		return E_FAIL;
	ADD_Child(m_pItemIcon);

	CIngredientList::INGREDIENT_LIST_DESC IngredientListDesc = {};
	IngredientListDesc.pParent = this;
	IngredientListDesc.iIngredientCount = 5;
	IngredientListDesc.vScale = { vScale.x - 10.f, vScale.y * 0.4f, 0.f };
	IngredientListDesc.vPosition = { 0.f, IngredientListDesc.vScale.y * 0.5f, 0.f};
	m_pIngredientList = CIngredientList::Create(m_pGraphic_Device, m_pDeviceContext);
	m_pIngredientList->SetZOrder(fParentZorder + 1);
	if (FAILED(m_pIngredientList->Initialize(&IngredientListDesc)))
		return E_FAIL;
	ADD_Child(m_pIngredientList);

	return S_OK;
}

CCreateToolTipUI* CCreateToolTipUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCreateToolTipUI* pCreateToolTipUI = new CCreateToolTipUI(pDevice, pContext);
	if (FAILED(pCreateToolTipUI->Initalize_Prototype()))
	{
		Safe_Release(pCreateToolTipUI);
		MSG_BOX("CREATE FAIL : CREATE TOOL TIP UI");
	}
	return pCreateToolTipUI;
}

CGameObject* CCreateToolTipUI::Clone(void* pArg)
{
	CCreateToolTipUI* pCreateToolTipUI = new CCreateToolTipUI(*this);
	if (FAILED(pCreateToolTipUI->Initialize(pArg)))
	{
		Safe_Release(pCreateToolTipUI);
		MSG_BOX("CLONE FAIL : CREATE TOOL TIP UI");
	}
	return pCreateToolTipUI;
}

void CCreateToolTipUI::Free()
{
	__super::Free();

	Safe_Release(m_pItemIcon);
	Safe_Release(m_pIngredientList);
}
