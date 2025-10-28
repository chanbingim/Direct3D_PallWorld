#include "WeaponQuickSlot.h"

#include "GameInstance.h"

#include "PlayerManager.h"
#include "ItemManager.h"
#include "ItemBase.h"

#include "WeaponSlotIcon.h"
#include "WeaponQuickSlotName.h"
#include "WeaponQuickSlotCount.h"


CWeaponQuickSlot::CWeaponQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CWeaponQuickSlot::CWeaponQuickSlot(const CWeaponQuickSlot& rhs) :
	CBackGround(rhs)
{
}

HRESULT CWeaponQuickSlot::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponQuickSlot::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_vImageColor = { 0.21f, 0.32f, 0.36f, 0.5f };
	m_iZOrder = 0;
	return S_OK;
}

void CWeaponQuickSlot::Update(_float fDeletaTime)
{
	auto pSelectWepaon = CPlayerManager::GetInstance()->GetCurrentSlotItemInfo(EUQIP_TYPE::WEAPON);
	if (pSelectWepaon)
	{
		auto ItemTexture = CItemManager::GetInstance()->GetItemTexture(CItemManager::ITEM_TEXTURE_TYPE::WEAPON, pSelectWepaon->GetItemData().iItemNum);
		m_pWeaponSlotIcon->SetIconTexture(ItemTexture);
		m_pWeaponSlotName->SetItemName(pSelectWepaon->GetItemData().szItemName);
	}
	else
	{
		m_pWeaponSlotIcon->SetIconTexture(nullptr);
		m_pWeaponSlotName->SetItemName(TEXT(""));
		m_pWeaponSlotCount->SetItemIndex(-1, -1);
	}

	for (auto iter : m_pChildList)
		iter->Update(fDeletaTime);

}

void CWeaponQuickSlot::Late_Update(_float fDeletaTime)
{
	for (auto iter : m_pChildList)
		iter->Late_Update(fDeletaTime);

	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CWeaponQuickSlot::Render()
{
	//이제 전부 지오메트리로 그릴거임
	Apply_ConstantShaderResources();
	m_pShaderCom->Bind_RawValue("g_vColor", &m_vImageColor, sizeof(_float4));

	m_pShaderCom->Update_Shader(4);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CWeaponQuickSlot::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_WeaponQuickSlot_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponQuickSlot::ADD_Childs()
{
	_float3 vScale = m_pTransformCom->GetScale();
	CUserInterface::GAMEOBJECT_DESC pUIDesc = {};
	pUIDesc.pParent = this;

	pUIDesc.vScale = { vScale.x * 0.6f , vScale.y, 0.f };
	pUIDesc.vPosition = { -30.f, 0.f, 0.f };
	/* 여기서 만들어서 자식으로 추가한다. */
	m_pWeaponSlotIcon = CWeaponSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
	if (nullptr == m_pWeaponSlotIcon)
		return E_FAIL;
	m_pWeaponSlotIcon->Initialize(&pUIDesc);

	pUIDesc.vScale = { vScale.x , 30.f, 0.f };
	pUIDesc.vPosition = {0, -vScale.y * 0.75f, 0.f};
	m_pWeaponSlotName = CWeaponQuickSlotName::Create(m_pGraphic_Device, m_pDeviceContext);
	if (nullptr == m_pWeaponSlotName)
		return E_FAIL;
	m_pWeaponSlotName->Initialize(&pUIDesc);

	pUIDesc.vScale = { vScale.x * 0.3f , vScale.y, 0.f };
	pUIDesc.vPosition = { 25.f, 0.f, 0.f };
	m_pWeaponSlotCount = CWeaponQuickSlotCount::Create(m_pGraphic_Device, m_pDeviceContext);
	if (nullptr == m_pWeaponSlotCount)
		return E_FAIL;
	m_pWeaponSlotCount->Initialize(&pUIDesc);

	ADD_Child(m_pWeaponSlotIcon);
	ADD_Child(m_pWeaponSlotName);
	ADD_Child(m_pWeaponSlotCount);
	return S_OK;
}

CWeaponQuickSlot* CWeaponQuickSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeaponQuickSlot* pWeaponquickSlot = new CWeaponQuickSlot(pDevice, pContext);
	if (FAILED(pWeaponquickSlot->Initalize_Prototype()))
	{
		Safe_Release(pWeaponquickSlot);
		MSG_BOX("CREATE FAIL : WEAPON QUICK SLOT");
	}

	return pWeaponquickSlot;
}

CGameObject* CWeaponQuickSlot::Clone(void* pArg)
{
	CGameObject* pWeaponquickSlot = new CWeaponQuickSlot(*this);
	if (FAILED(pWeaponquickSlot->Initialize(pArg)))
	{
		Safe_Release(pWeaponquickSlot);
		MSG_BOX("CLONE FAIL : WEAPON QUICK SLOT");
	}

	return pWeaponquickSlot;
}

void CWeaponQuickSlot::Free()
{
	__super::Free();

	Safe_Release(m_pWeaponSlotIcon);
	Safe_Release(m_pWeaponSlotName);
	Safe_Release(m_pWeaponSlotCount);
}
