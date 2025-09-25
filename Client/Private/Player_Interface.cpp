#include "Player_Interface.h"

#include "GameInstance.h"
#include "HealthBar.h"
#include "BackGround.h"

#include "PlayerManager.h"

#include "GameData_Manager.h"
#include "GamePlayHUD.h"

CPlayer_Interface::CPlayer_Interface(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CUserInterface(pGraphic_Device, pDeviceContext)
{
}

CPlayer_Interface::CPlayer_Interface(const CPlayer_Interface& rhs) :
	CUserInterface(rhs)
{
}

HRESULT CPlayer_Interface::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Interface::Initialize(void* pArg)
{
	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;

	m_eType = OBJECT_TYPE::STATIC;

	return S_OK;
}

void CPlayer_Interface::Update(_float fDeletaTime)
{
	auto PlayerInfo = CPlayerManager::GetInstance()->GetPlayerData();

	m_pHpBar->SetPercent(PlayerInfo.CurHealth / PlayerInfo.MaxHealth);
	m_pHugerBar->SetPercent(PlayerInfo.CurHunger / PlayerInfo.MaxHunger);
	m_pGuardBar->SetPercent(PlayerInfo.ShieldPoint / PlayerInfo.ShieldPoint);

}

void CPlayer_Interface::Late_Update(_float fDeletaTime)
{
}

HRESULT CPlayer_Interface::Render()
{
	return S_OK;
}

HRESULT CPlayer_Interface::ADD_Components()
{
	return S_OK;
}

HRESULT CPlayer_Interface::ADD_Childs()
{
	auto pDefaultHUD = m_pGameInstance->GetCurrentHUD();
	if (nullptr == pDefaultHUD)
		return E_FAIL;

	auto pInGame_HUD = dynamic_cast<CGamePlayHUD*>(pDefaultHUD);
	if (pInGame_HUD)
	{
#pragma region Icon
		CBackGround::GAMEOBJECT_DESC IconDesc;
		IconDesc.pParent = this;
		IconDesc.vScale = { 25.f, 25.f, 0.f };

		//Health Icon
		IconDesc.vPosition = { -75.f, 75.f, 0.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Health_Icon"), TEXT("HealthIcon"), &IconDesc)))
			return E_FAIL;

		//Hunger Icon
		IconDesc.vPosition = { -75.f, 100.f, 0.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Hunger_Icon"), TEXT("HungerIcon"), &IconDesc)))
			return E_FAIL;
#pragma endregion

#pragma region Bar
		CProgressBar::PROGRESS_DESC Desc = {};
		Desc.pParent = this;
		Desc.vScale = { 200.f, 20.f, 0.f };

		//Guard bar
		Desc.vPosition = { 50.f, 50.f, 0.f };
		Desc.vColor = { 0.f, 1.f, 1.f, 1.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Guard_Bar"), TEXT("GuardBar"), &Desc, (CUserInterface**)&m_pGuardBar)))
			return E_FAIL;

		//Health bar
		Desc.vPosition = { 50.f, 75.f, 0.f };
		Desc.vColor = { 1.f, 0.f, 0.f, 1.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Health_Bar"), TEXT("HealthBar"), &Desc, (CUserInterface**)&m_pHpBar)))
			return E_FAIL;

		//Hunger bar
		Desc.vPosition = { 50.f, 100.f, 0.f };
		Desc.vColor = { 250 / 255.f, 124 / 255.f, 35 / 255.f, 1.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Health_Bar"), TEXT("HungerBar"), &Desc, (CUserInterface**)&m_pHugerBar)))
			return E_FAIL;
#pragma endregion

#pragma region Battle_Pell_UI
		IconDesc.vScale = { 75.f, 75.f, 0.f };
		//Health Icon
		IconDesc.vPosition = { 30.f, -45.f, 0.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Battle_Pell_UI"), TEXT("Battle_Pell_UI"), &IconDesc)))
			return E_FAIL;
#pragma endregion

#pragma region WEAPON_UI
		IconDesc.vScale = { 200.f, 65.f, 0.f };
		//Health Icon
		IconDesc.vPosition = { g_iWinSizeX * 0.69f + (IconDesc.vScale.x * 0.8f),
							   IconDesc.vScale.y + 30.f,
							   0.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Weapon_QuickSlot"), TEXT("Weapon_Slot_UI"), &IconDesc)))
			return E_FAIL;
#pragma endregion

#pragma region ITEM_SLOT_UI
		IconDesc.vScale = { 100.f, 65.f, 0.f };
		//Health Icon
		IconDesc.vPosition = { g_iWinSizeX * 0.63f + ( IconDesc.vScale.x * 0.8f),
								IconDesc.vScale.y + 30.f,
								0.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Item_QuickSlot"), TEXT("Item_Slot_UI"), &IconDesc)))
			return E_FAIL;

#pragma endregion

	}

	return S_OK;
}

CPlayer_Interface* CPlayer_Interface::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
	CPlayer_Interface* pPlayer_Interface = new CPlayer_Interface(pGraphic_Device, pDeviceContext);
	if (FAILED(pPlayer_Interface->Initalize_Prototype()))
	{
		Safe_Release(pPlayer_Interface);
		MSG_BOX("CREATE FAIL : PLAYER INTERFACE");
	}

	return pPlayer_Interface;
}

CGameObject* CPlayer_Interface::Clone(void* pArg)
{
	CPlayer_Interface* pPlayer_Interface = new CPlayer_Interface(*this);
	if (FAILED(pPlayer_Interface->Initialize(pArg)))
	{
		Safe_Release(pPlayer_Interface);
		MSG_BOX("CLONE FAIL : PLAYER INTERFACE");
	}

	return pPlayer_Interface;
}

void CPlayer_Interface::Free()
{
	__super::Free();
}
