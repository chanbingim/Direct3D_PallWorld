#include "PlayerWeaponSlot.h"

#include "GameInstance.h"
#include "PlayerManager.h"
#include "ItemBase.h"
#include "Player.h"

CPlayerWeaponSlot::CPlayerWeaponSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPlayerItemSlot(pDevice, pContext)
{
}

CPlayerWeaponSlot::CPlayerWeaponSlot(const CPlayerWeaponSlot& rhs) :
	CPlayerItemSlot(rhs)
{
}

HRESULT CPlayerWeaponSlot::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerWeaponSlot::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	WEAPON_SLOT_DESC* SlotDesc = static_cast<WEAPON_SLOT_DESC*>(pArg);
	m_pLeftSocket = SlotDesc->pLeftSocket;

	return S_OK;
}

void CPlayerWeaponSlot::Priority_Update(_float fDeletaTime)
{
	
}

void CPlayerWeaponSlot::Update(_float fDeletaTime)
{
	m_CurrentEuipItemInfo = CPlayerManager::GetInstance()->GetSelectItemData();
	ChangeModelBuffer(CPlayerManager::GetInstance()->GetCurrentSelectItem(), false);

	if (m_CurrentEuipItemInfo)
	{
		const ITEM_DESC& ItemData = m_CurrentEuipItemInfo->GetItemData();
		if (ITEM_TYPE::EQUIPMENT == ItemData.ItemType)
			m_LeftFlag = ItemData.TypeDesc.EuqipDesc.bIsLeftSocket;
		m_pCollision[0]->SetCollision(m_CurrentEuipItemInfo->GetItemData().TypeDesc.EuqipDesc.vCenter, m_CurrentEuipItemInfo->GetItemData().TypeDesc.EuqipDesc.vExtents);

		m_bIsAnimWeapon = ItemData.IsAnimModel;
		if (m_bIsAnimWeapon)
		{
			if (ITEM_TYPE::EQUIPMENT == ItemData.ItemType)
			{
				if (ItemData.TypeDesc.EuqipDesc.bIsChargeAble)
					ChangeAnimWaponAnimationIndex();
			}

			if (ItemData.IsPlayAnimation)
				m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime);
			else
				m_pVIBufferCom->PlayAnimation(0, 0, 0);
		}
	}
	else
	{
		m_pCollision[0]->SetCollision({ 0, 0, 0 }, { 0.1f, 0.1f, 0.1f });
		m_pCollision[1]->SetCollision({ 0, 0, 0 }, { 0.1f, 0.1f, 0.1f });
	}
}

void CPlayerWeaponSlot::Late_Update(_float fDeletaTime)
{
	if (nullptr == m_pVIBufferCom)
	{
		UpdateCombinedMatrix(m_pLeftSocket);
		m_pCollision[1]->UpdateColiision(XMLoadFloat4x4(&m_CombinedWorldMatrix));

		UpdateCombinedMatrix();
		m_pCollision[0]->UpdateColiision(XMLoadFloat4x4(&m_CombinedWorldMatrix));

		for (auto i = 0; i < 2; ++i)
			m_pGameInstance->ADD_CollisionList(m_pCollision[i]);
	}
	else
	{
		if (m_LeftFlag)
			UpdateCombinedMatrix(m_pLeftSocket);
		else
			UpdateCombinedMatrix();

		m_pCollision[0]->UpdateColiision(XMLoadFloat4x4(&m_CombinedWorldMatrix));
		m_pGameInstance->ADD_CollisionList(m_pCollision[0]);
	}

	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayerWeaponSlot::Render()
{
	__super::Render();

	if (nullptr == m_pVIBufferCom)
	{
		for(auto  i =0; i < 2; ++i)
			m_pCollision[i]->Render({});
	}
	else
		m_pCollision[0]->Render({});


	return S_OK;
}

HRESULT CPlayerWeaponSlot::ADD_Components()
{
	CBoxCollision::BOX_COLLISION_DESC BoxColDesc = {};
	BoxColDesc.pOwner = this;
	BoxColDesc.Extents = { 0.1f, 0.1f, 0.1f };
	BoxColDesc.vCneter = {};

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_CoolisionBox"), TEXT("CollisionR_Com"), (CComponent**)&m_pCollision[0], &BoxColDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_CoolisionBox"), TEXT("CollisionL_Com"), (CComponent**)&m_pCollision[1], &BoxColDesc)))
		return E_FAIL;

	for (auto i = 0; i < 2; ++i)
	{
		m_pCollision[i]->ADD_IgnoreObejct(typeid(CPlayerWeaponSlot).hash_code());
		m_pCollision[i]->ADD_IgnoreObejct(typeid(CPlayer).hash_code());

		m_pCollision[i]->BindBeginOverlapEvent([&](_float3 vDir, CGameObject* pHitActor) { this->HitBegin(vDir, pHitActor); });
	}

	return S_OK;
}

void CPlayerWeaponSlot::HitBegin(_float3 vDir, CGameObject* pHitActor)
{
	auto HitObejct = dynamic_cast<CContainerObject*>(pHitActor);
	if (HitObejct)
	{
		HitObejct->Damage(nullptr, CPlayerManager::GetInstance()->GetCurrentPlayer());
	}
}

CPlayerWeaponSlot* CPlayerWeaponSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerWeaponSlot* pPlayerWeaponSlot = new CPlayerWeaponSlot(pDevice, pContext);
	if (FAILED(pPlayerWeaponSlot->Initalize_Prototype()))
	{
		Safe_Release(pPlayerWeaponSlot);
		MSG_BOX("CREATE FAIL : PLAYER WEAPON SLOT");
	}
	return pPlayerWeaponSlot;
}

CGameObject* CPlayerWeaponSlot::Clone(void* pArg)
{
	CPlayerWeaponSlot* pPlayerWeaponSlot = new CPlayerWeaponSlot(*this);
	if (FAILED(pPlayerWeaponSlot->Initialize(pArg)))
	{
		Safe_Release(pPlayerWeaponSlot);
		MSG_BOX("CLONE FAIL : PLAYER WEAPON SLOT");
	}
	return pPlayerWeaponSlot;
}

void CPlayerWeaponSlot::Free()
{
	__super::Free();

	for (auto i = 0; i < 2; ++i)
		Safe_Release(m_pCollision[i]);

}