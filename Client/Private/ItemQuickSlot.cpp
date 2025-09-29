#include "ItemQuickSlot.h"

#include "PlayerManager.h"
#include "ItemManager.h"
#include "ItemBase.h"

#include "GameInstance.h"
#include "ItemQuickSlotCount.h"
#include "ItemQuickSlotIcon.h"
#include "ItemQuickSlotName.h"


CItemQuickSlot::CItemQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CItemQuickSlot::CItemQuickSlot(const CItemQuickSlot& rhs) :
	CBackGround(rhs)
{
}

HRESULT CItemQuickSlot::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItemQuickSlot::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_iZOrder = 0;
	m_vImageColor = { 0.21f, 0.32f, 0.36f, 0.5f };

	return S_OK;
}

void CItemQuickSlot::Update(_float fDeletaTime)
{
	/*auto pSelectItem = CPlayerManager::GetInstance()->GetSelectItemData();
	if (pSelectItem)
	{
		auto ItemTexture = CItemManager::GetInstance()->GetItemTexture(pSelectItem->GetItemData().iItemNum);
		m_pQuickSlotIcon->SetItemTypeIcon(ItemTexture);
		m_pQuickSlotName->SetItemName(pSelectItem->GetItemData().szItemName);
	}
	else
	{
		m_pQuickSlotIcon->SetItemTypeIcon(nullptr);
		m_pQuickSlotName->SetItemName(TEXT(""));
		m_pQuickSlotCount->SetItemIndex(-1, -1);
	}*/

	for (auto iter : m_pChildList)
		iter->Update(fDeletaTime);
}

void CItemQuickSlot::Late_Update(_float fDeletaTime)
{
	for (auto iter : m_pChildList)
		iter->Late_Update(fDeletaTime);

	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CItemQuickSlot::Render()
{
	//이제 전부 지오메트리로 그릴거임
	Apply_ConstantShaderResources();
	m_pShaderCom->Bind_RawValue("g_vColor", &m_vImageColor, sizeof(_float4));

	m_pShaderCom->Update_Shader(4);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CItemQuickSlot::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_WeaponQuickSlot_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItemQuickSlot::ADD_Childs()
{
	_float3 vScale = m_pTransformCom->GetScale();
	CUserInterface::GAMEOBJECT_DESC pUIDesc = {};
	pUIDesc.pParent = this;

	pUIDesc.vScale = { vScale.x * 0.6f , vScale.y, 0.f };
	pUIDesc.vPosition = { -50.f, 0.f, 0.f };
	/* 여기서 만들어서 자식으로 추가한다. */
	m_pQuickSlotIcon = CItemQuickSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
	if (nullptr == m_pQuickSlotIcon)
		return E_FAIL;
	m_pQuickSlotIcon->Initialize(&pUIDesc);

	pUIDesc.vScale = { vScale.x , 30.f, 0.f };
	pUIDesc.vPosition = { 0, -vScale.y * 0.75f, 0.f };
	m_pQuickSlotName = CItemQuickSlotName::Create(m_pGraphic_Device, m_pDeviceContext);
	if (nullptr == m_pQuickSlotName)
		return E_FAIL;
	m_pQuickSlotName->Initialize(&pUIDesc);

	pUIDesc.vScale = { vScale.x * 0.3f , vScale.y, 0.f };
	pUIDesc.vPosition = { 10.f, 0.f, 0.f };
	m_pQuickSlotCount = CItemQuickSlotCount::Create(m_pGraphic_Device, m_pDeviceContext);
	if (nullptr == m_pQuickSlotCount)
		return E_FAIL;
	m_pQuickSlotCount->Initialize(&pUIDesc);

	ADD_Child(m_pQuickSlotIcon);
	ADD_Child(m_pQuickSlotName);
	ADD_Child(m_pQuickSlotCount);
	return S_OK;
}

CItemQuickSlot* CItemQuickSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItemQuickSlot* pItemQuickSlot = new CItemQuickSlot(pDevice, pContext);
	if (FAILED(pItemQuickSlot->Initalize_Prototype()))
	{
		Safe_Release(pItemQuickSlot);
		MSG_BOX("CREATE FAIL : ITEM QUICK SLOT");
	}
	return pItemQuickSlot;
}

CGameObject* CItemQuickSlot::Clone(void* pArg)
{
	CItemQuickSlot* pItemQuickSlot = new CItemQuickSlot(*this);
	if (FAILED(pItemQuickSlot->Initialize(pArg)))
	{
		Safe_Release(pItemQuickSlot);
		MSG_BOX("CLONE FAIL : ITEM QUICK SLOT");
	}
	return pItemQuickSlot;
}

void CItemQuickSlot::Free()
{
	__super::Free();

	Safe_Release(m_pQuickSlotIcon);
	Safe_Release(m_pQuickSlotName);
	Safe_Release(m_pQuickSlotCount);
}
