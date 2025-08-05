#include "Player_Interface.h"

#include "GameInstance.h"
#include "HealthBar.h"

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

	m_pCharacterInfo = static_cast<CHARACTER_DESC*>(CGameData_Manager::GetInstance()->GetPlayerInfo());


	return S_OK;
}

void CPlayer_Interface::Update(_float fDeletaTime)
{
	m_pHpBar->SetPercent(0.5f);
	if (m_pCharacterInfo)
	{
		m_pHpBar->SetPercent(m_pCharacterInfo->CurHealth / m_pCharacterInfo->MaxHealth);
		m_pHugerBar->SetPercent(m_pCharacterInfo->CurHunger / m_pCharacterInfo->MaxHunger);
	}
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
		CProgressBar::PROGRESS_DESC Desc = {};
		Desc.pParent = this;
		Desc.vScale = { 200.f, 25.f, 0.f };
		Desc.vPosition = { 50, 50.f, 0.f };
		Desc.vColor = { 1.f, 0.f, 0.f, 1.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Health_Bar"), TEXT("HealthBar"), &Desc, (CUserInterface**)&m_pHpBar)))
			return E_FAIL;

		Desc.vPosition = { 50.f, 80.f, 0.f };
		Desc.vColor = { 250/255.f, 124/255.f, 35/255.f, 1.f };
		if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Health_Bar"), TEXT("HungerBar"), &Desc, (CUserInterface**)&m_pHugerBar)))
			return E_FAIL;
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
