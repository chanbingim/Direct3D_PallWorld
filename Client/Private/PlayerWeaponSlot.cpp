#include "PlayerWeaponSlot.h"

#include "GameInstance.h"
#include "PlayerManager.h"

#include "ItemBase.h"

#include "PlayerStateMachine.h"

#include "ProjectileObject.h"
#include "ProjectileSlot.h"
#include "Enviormnent.h"

#include "TrailEffect.h"
#include "PellBase.h"
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

	CTrailEffect::TRAIL_EFFECT_DESC TrailDesc = {};
	TrailDesc.pSocketMatrix = &m_CombinedWorldMatrix;
	TrailDesc.TrailDisPatch = { 4, 4, 2 };
	TrailDesc.szTrailEffectName = TEXT("T_Trail04.png");

	m_pTrail = static_cast<CTrailEffect*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trail_Defalut"), &TrailDesc));
	m_pTrail->SetRenderTrail(true);

	return S_OK;
}

void CPlayerWeaponSlot::Priority_Update(_float fDeletaTime)
{
	m_pProjectileSlot->Priority_Update(fDeletaTime);
	m_pTrail->Priority_Update(fDeletaTime);
}

void CPlayerWeaponSlot::Update(_float fDeletaTime)
{
	m_CurrentEuipItemInfo = CPlayerManager::GetInstance()->GetCurrentSlotItemInfo(EUQIP_TYPE::WEAPON);
	ChangeModelBuffer(CPlayerManager::GetInstance()->GetCurrentSlotItemModel(EUQIP_TYPE::WEAPON), false);

	if (m_CurrentEuipItemInfo)
	{
		const ITEM_DESC& ItemData = m_CurrentEuipItemInfo->GetItemData();
		if (ITEM_TYPE::EQUIPMENT == ItemData.ItemType)
		{
			m_LeftFlag = ItemData.TypeDesc.EuqipDesc.bIsLeftSocket;
			m_pProjectileSlot->SetItemIndex(ItemData.TypeDesc.EuqipDesc.iProjectileItemIndex);
		}

		m_pCollision[0]->SetCollision(m_CurrentEuipItemInfo->GetItemData().TypeDesc.EuqipDesc.vCenter, { 0.f, 0.f, 0.f, 0.f }, m_CurrentEuipItemInfo->GetItemData().TypeDesc.EuqipDesc.vExtents);
		m_bIsAnimWeapon = ItemData.IsAnimModel;
		if (m_bIsAnimWeapon)
		{
			if (ITEM_TYPE::EQUIPMENT == ItemData.ItemType)
			{
				if (WEAPON_STATE::CHARGE == m_eState && m_bIsAnimFinished)
					m_eState = WEAPON_STATE::CHARGE_LOOP;

				if (ItemData.TypeDesc.EuqipDesc.bIsChargeAble)
					ChangeAnimWaponAnimationIndex();
			}

			if (ItemData.IsPlayAnimation)
				m_bIsAnimFinished = m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime, 10.f , m_bIsAnimLoop);
			else
				m_pVIBufferCom->PlayAnimation(0, 0, 0);
		}
	}
	else
	{
		m_pCollision[0]->SetCollision({ 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f }, { 0.1f, 0.1f, 0.1f });
		m_pCollision[1]->SetCollision({ 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f }, { 0.1f, 0.1f, 0.1f });
	}
	m_pTrail->Update(fDeletaTime);
	m_pProjectileSlot->Update(fDeletaTime);
}

void CPlayerWeaponSlot::Late_Update(_float fDeletaTime)
{
	if (nullptr == m_pVIBufferCom)
	{
		UpdateCombinedMatrix(m_pLeftSocket);
		m_pCollision[1]->UpdateColiision(XMLoadFloat4x4(&m_CombinedWorldMatrix));

		UpdateCombinedMatrix();
		m_pCollision[0]->UpdateColiision(XMLoadFloat4x4(&m_CombinedWorldMatrix));
	}
	else
	{
		if (m_LeftFlag)
			UpdateCombinedMatrix(m_pLeftSocket);
		else
			UpdateCombinedMatrix();

		m_pCollision[0]->UpdateColiision(XMLoadFloat4x4(&m_CombinedWorldMatrix));
	}

	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
	m_pTrail->Late_Update(fDeletaTime);
	m_pProjectileSlot->Late_Update(fDeletaTime);
}

HRESULT CPlayerWeaponSlot::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		for(auto  i =0; i < 2; ++i)
			m_pCollision[i]->Render({});
	}
	else
	{
		__super::Render();
		m_pCollision[0]->Render({});
	}

	return S_OK;
}

HRESULT CPlayerWeaponSlot::ShootProjecttileObject()
{
	if (nullptr == m_CurrentEuipItemInfo)
		return E_FAIL;

	auto EuipInfo = m_CurrentEuipItemInfo->GetItemData();
	if (!lstrcmp(EuipInfo.TypeDesc.EuqipDesc.ProjectilePrototpyeName, TEXT("")))
		return E_FAIL;

	CPlayerStateMachine::PLAYER_STATE playerState = {};
	CPlayer* CurrentPlayer = CPlayerManager::GetInstance()->GetCurrentPlayer();
	CurrentPlayer->GetPlayerState(&playerState);

	CProjectileObject::PROJECTILE_DESC ProjectileDesc = {};

	//이함수를 통해서 프로젝타일 위치에 생성해서 날린다.
	ProjectileDesc.vPosition = *reinterpret_cast<_float3*>(m_CombinedWorldMatrix.m[3]);
	ProjectileDesc.vScale = { 1.f, 1.f, 1.f };
	ProjectileDesc.pAttacker = CurrentPlayer;

	if (playerState.bIsAiming)
	{
		_vector FarPoint = m_pGameInstance->GetCameraState(WORLDSTATE::POSITION) + m_pGameInstance->GetCameraState(WORLDSTATE::LOOK) * m_pGameInstance->GetCameraINFO().y;
		XMStoreFloat3(&ProjectileDesc.vDireaction, XMVector3Normalize(FarPoint - XMLoadFloat3(&ProjectileDesc.vPosition)));
	}
	else
	{
		XMStoreFloat3(&ProjectileDesc.vDireaction, CurrentPlayer->GetTransform()->GetLookVector());
	}

	ProjectileDesc.vThrowSpeed = 10.f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), EuipInfo.TypeDesc.EuqipDesc.ProjectilePrototpyeName,
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Projectile"), &ProjectileDesc)))
		return E_FAIL;

	return S_OK;
}

void CPlayerWeaponSlot::NearAttackOnCollision()
{
	if (nullptr == m_pVIBufferCom)
	{
		for (auto i = 0; i < 2; ++i)
			m_pGameInstance->ADD_CollisionList(m_pCollision[i]);
	}
	else
	{
		if(WEAPON::AXE >= m_CurrentEuipItemInfo->GetItemData().TypeDesc.EuqipDesc.Weapon_Type)
			m_pGameInstance->ADD_CollisionList(m_pCollision[0]);
	}
		
}

HRESULT CPlayerWeaponSlot::ADD_Components()
{
	COBBCollision::OBB_COLLISION_DESC OBBColDesc = {};
	OBBColDesc.pOwner = this;
	OBBColDesc.vExtents = { 0.1f, 0.1f, 0.1f };
	OBBColDesc.vCneter = {};

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("CollisionR_Com"), (CComponent**)&m_pCollision[0], &OBBColDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("CollisionL_Com"), (CComponent**)&m_pCollision[1], &OBBColDesc)))
		return E_FAIL;

	CProjectileSlot::PROJECTILE_DESC ProjectileDesc = {};
	ProjectileDesc.pParent = m_pParent;
	lstrcpy(ProjectileDesc.ObjectTag, TEXT("ProjectileSocket"));
	ProjectileDesc.vScale = { 1.f, 1.f, 1.f };
	ProjectileDesc.SocketMatrix = m_SocketMatrix;
	ProjectileDesc.fLeftSocket = m_pLeftSocket;

	m_pProjectileSlot = CProjectileSlot::Create(m_pGraphic_Device, m_pDeviceContext);
	if(FAILED(m_pProjectileSlot->Initialize(&ProjectileDesc)))
		return E_FAIL;

	for (auto i = 0; i < 2; ++i)
	{
		m_pCollision[i]->ADD_IgnoreObejct(typeid(CPlayerWeaponSlot).hash_code());
		m_pCollision[i]->ADD_IgnoreObejct(typeid(CPlayer).hash_code());

		m_pCollision[i]->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { this->HitBegin(vDir, pHitActor); });
	}

	return S_OK;
}

void CPlayerWeaponSlot::HitBegin(_float3 vDir, CGameObject* pHitActor)
{
	_bool bIsDamage = false;
	auto HitObejct = dynamic_cast<CContainerObject*>(pHitActor);
	if (HitObejct)
	{
		auto pPellBase = dynamic_cast<CPellBase*>(pHitActor);
		if(pPellBase)
		{
			switch (pPellBase->GetPellTeam())
			{
			case ACTOR_TEAM::NEUTRAL:
			case ACTOR_TEAM::ENEMY :
				bIsDamage = true;
				break;
			case ACTOR_TEAM::FRENDLY:
				return;
			}
		}
		else
		{
			bIsDamage = true;
		}
	}

	auto EnvObject = dynamic_cast<CEnviormnent*>(pHitActor);
	if (EnvObject)
		bIsDamage = true;

	if (bIsDamage)
	{
		DEFAULT_DAMAGE_DESC DamageDes = {};
		DamageDes.fDmaged = 10.f;

		static_cast<CActor *>(pHitActor)->Damage(&DamageDes, CPlayerManager::GetInstance()->GetCurrentPlayer());
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

	Safe_Release(m_pTrail);
	Safe_Release(m_pProjectileSlot);
	for (auto i = 0; i < 2; ++i)
		Safe_Release(m_pCollision[i]);

}