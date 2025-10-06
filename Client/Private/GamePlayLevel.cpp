#include "GamePlayLevel.h"

#include "GameInstance.h"
#include "GamePlayHUD.h"

#include "TerrainManager.h"
#include "Enviormnent.h"
#include "ItemManager.h"
#include "PlayerManager.h"
#include "PellManager.h"
#include "Light.h"

CGamePlayLevel::CGamePlayLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID) :
	CLevel(pDevice, pContext, _iID)
{
}

HRESULT CGamePlayLevel::Initialize()
{
	CLight::LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT::DIRECTION;
	LightDesc.vDiffuse = _float4(1.f, 1.0f, 1.0f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);

	m_pGameInstance->ADDLight(CLight::Create(LightDesc));

	if (FAILED(ADD_CameraLayer(TEXT("Layer_GamePlay_Camera"))))
		return E_FAIL;

	if (FAILED(ADD_TerrianLayer(TEXT("Layer_GamePlay_Terrian"))))
		return E_FAIL;

	CTerrainManager::GetInstance()->Initialize(nullptr);
	if (FAILED(ADD_SkyLayer(TEXT("Layer_GamePlay_SKY"))))
		return E_FAIL;

	CPlayerManager::PLAYER_MANAGER_DESC PlayerDesc;
	PlayerDesc.iMaxInvenWeight = 1000;
	PlayerDesc.iNumInvenMaxSlot = 60;
	CPlayerManager::GetInstance()->Initialize(&PlayerDesc);
	if (FAILED(ADD_PlayerLayer(TEXT("Layer_GamePlay_Player"))))
		return E_FAIL;

	if (FAILED(ADD_EnviornmentLayer(TEXT("Layer_GamePlay_Enviorment"))))
		return E_FAIL;

	CPellManager::GetInstance()->Initialize(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(ADD_PellLayer(TEXT("Layer_GamePlay_Pell"))))
		return E_FAIL;

	if (FAILED(Setting_GamePlayHUD()))
		return E_FAIL;

	m_pGameInstance->ShowInGameMouse(VISIBILITY::HIDDEN);

	return S_OK;
}

void CGamePlayLevel::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);


	
}

HRESULT CGamePlayLevel::Render()
{
	SetWindowText(g_hWnd, TEXT("게임 플레이 레벨"));
	return S_OK;
}

HRESULT CGamePlayLevel::ADD_CameraLayer(const _wstring& LayerName)
{
	CBaseCamera::CAMERA_DESC Camera_Desc = {};

	Camera_Desc.vEye = { 0.f, 20.f, -20.f };
	Camera_Desc.vAt = { 0.f, 0.f, 0.f};

	Camera_Desc.fFov = XMConvertToRadians(60.f);
	Camera_Desc.fNear = 0.1f;
	Camera_Desc.fFar = 1000.f;

	if(FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera"),
				ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Camera_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlayLevel::ADD_TerrianLayer(const _wstring& LayerName)
{
	CGameObject::GAMEOBJECT_DESC Desc;
	ZeroMemory(&Desc, sizeof(CGameObject::GAMEOBJECT_DESC));
	Desc.vScale = { 1.f, 1.f, 1.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DefualtMap"),
				ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlayLevel::ADD_SkyLayer(const _wstring& LayerName)
{
	CGameObject::GAMEOBJECT_DESC Desc;
	ZeroMemory(&Desc, sizeof(CGameObject::GAMEOBJECT_DESC));
	Desc.vScale = { 1.f, 1.f, 1.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SkyBox"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlayLevel::ADD_EnviornmentLayer(const _wstring& LayerName)
{
	CGameObject::GAMEOBJECT_DESC Desc = {};
	Desc.vScale = { 1.f, 1.f, 1.f };
	Desc.vPosition = { 100.f, -64.f, 890.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_SmallGrass"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	Desc.vPosition = { 100.f, 14.f, 900.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_Flower"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	CEnviormnent::ENVIORMNENT_DESC EnviDesc = {};
	EnviDesc.vScale = { 1.f, 1.f,1.f };
	for (_uInt i = 0; i < 10; i++)
	{
		EnviDesc.iModelIndex = m_pGameInstance->Random(0.f, 2.f);
		
		EnviDesc.vPosition = { -84.f, 14.f, 515.f + 50 * i };
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rock"),
			ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &EnviDesc)))
			return E_FAIL;

		EnviDesc.iModelIndex = m_pGameInstance->Random(0.f, 1.f);
		EnviDesc.vPosition = { -114.f, 14.f, 505.f + 50 * i };
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Tree"),
			ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &EnviDesc)))
			return E_FAIL;

		EnviDesc.vPosition = { -114.f - 5 * i, 14.f, 495.f + 50 * i };
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PelJium"),
			ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &EnviDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CGamePlayLevel::ADD_PlayerLayer(const _wstring& LayerName)
{
	CGameObject::GAMEOBJECT_DESC Desc;
	ZeroMemory(&Desc, sizeof(CGameObject::GAMEOBJECT_DESC));
	Desc.vScale = { 1.f, 1.f, 1.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlayLevel::ADD_PellLayer(const _wstring& LayerName)
{
	CGameObject::GAMEOBJECT_DESC Desc;
	ZeroMemory(&Desc, sizeof(CGameObject::GAMEOBJECT_DESC));

	for (_uInt i = 0; i < 10; ++i)
	{
		wsprintf(Desc.ObjectTag, TEXT("Bed Cat"));
		Desc.vScale = { 1.f, 1.f, 1.f };
		Desc.vPosition = { m_pGameInstance->Random(100, 200.f), 1.f, m_pGameInstance->Random(100, 200.f) };
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BedCat"),
			ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
			return E_FAIL;
	}

	for (_uInt i = 0; i < 10; ++i)
	{
		wsprintf(Desc.ObjectTag, TEXT("Drorong"));
		Desc.vPosition = { m_pGameInstance->Random(100, 200.f), 1.f, m_pGameInstance->Random(100, 200.f) };
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Drorong"),
			ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
			return E_FAIL;
	}
	
	for (_uInt i = 0; i < 10; ++i)
	{
		wsprintf(Desc.ObjectTag, TEXT("ElectricPanda"));
		Desc.vPosition = { m_pGameInstance->Random(100, 200.f), 1.f, m_pGameInstance->Random(100, 200.f) };
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricPanda"),
			ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
			return E_FAIL;
	}
	
	/*
	wsprintf(Desc.ObjectTag, TEXT("Herorong"));
	Desc.vScale = { 0.05f, 0.05f, 0.05f };
	Desc.vPosition = { 30.f, 1.f, 30.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Herorong"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	wsprintf(Desc.ObjectTag, TEXT("FlowerLabbit"));
	Desc.vScale = { 0.1f, 0.1f, 0.1f };
	Desc.vPosition = { 50.f, 1.f, 50.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FlowerRabbit"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	wsprintf(Desc.ObjectTag, TEXT("Frog"));
	Desc.vScale = { 0.1f, 0.1f, 0.1f };
	Desc.vPosition = { 70.f, 1.f, 70.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Frog"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	wsprintf(Desc.ObjectTag, TEXT("Yeti"));
	Desc.vScale = { 0.05f, 0.05f, 0.05f };
	Desc.vPosition = { 100.f, 1.f, 20.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Yeti"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;*/
	return S_OK;
}

HRESULT CGamePlayLevel::Setting_GamePlayHUD()
{
	CGamePlayHUD* pGamePlayHUD = CGamePlayHUD::Create(m_pGraphic_Device, m_pDeviceContext);
	SetHUD(pGamePlayHUD);
	if (FAILED(pGamePlayHUD->Initialize()))
		return E_FAIL;

	return S_OK;
}

CGamePlayLevel* CGamePlayLevel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID)
{
	CGamePlayLevel* pGamePlayLevel = new CGamePlayLevel(pDevice, pContext, ENUM_CLASS(eID));
	if (nullptr == pGamePlayLevel)
	{
		Safe_Release(pGamePlayLevel);
		MSG_BOX("CREATE FAIL : GAME PLAY LEVEL");
	}
	return pGamePlayLevel;
}

void CGamePlayLevel::Free()
{
	__super::Free();
}
