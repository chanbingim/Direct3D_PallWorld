#include "GamePlayLevel.h"

#include "GameInstance.h"
#include "GamePlayHUD.h"

#include "Light.h"
#include "SunLight.h"
#include "ShadowCamera.h"

#include "StringHelper.h"
#include "CsvHelper.h"

#include "PellBase.h"
#include "Enviormnent.h"

#include "TerrainManager.h"
#include "FastTravelObject.h"
#include "Chunk.h"

#include "ItemManager.h"
#include "PlayerManager.h"
#include "PellManager.h"

CGamePlayLevel::CGamePlayLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID) :
	CLevel(pDevice, pContext, _iID)
{
}

HRESULT CGamePlayLevel::Initialize()
{
	if (FAILED(ADD_Light()))
		return E_FAIL;

	if (FAILED(ADD_CameraLayer(TEXT("Layer_GamePlay_Camera"))))
		return E_FAIL;

	if (FAILED(ADD_TerrianLayer(TEXT("Layer_GamePlay_Terrian"))))
		return E_FAIL;

	CTerrainManager::GetInstance()->Initialize(m_pGraphic_Device, m_pDeviceContext);
	if (FAILED(LoadMainArea()))
		return E_FAIL;

	if (FAILED(LoadPalArea()))
		return E_FAIL;

	CPlayerManager::PLAYER_MANAGER_DESC PlayerDesc;
	PlayerDesc.iMaxInvenWeight = 1000;
	PlayerDesc.iNumInvenMaxSlot = 60;
	CPlayerManager::GetInstance()->Initialize(&PlayerDesc);
	if (FAILED(ADD_PlayerLayer(TEXT("Layer_GamePlay_Player"))))
		return E_FAIL;

	if (FAILED(ADD_SkyLayer(TEXT("Layer_GamePlay_SKY"))))
		return E_FAIL;

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

HRESULT CGamePlayLevel::ADD_Light()
{

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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_CubeSkyBox"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	CSunLight::SUN_LIGHT_DESC SunLightDesc = {};
	SunLightDesc.fRadius = 150.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_SunLight"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("GamePlay_Layer_Dir_Light"), &SunLightDesc)))
		return E_FAIL;

	CShadowCamera::SHADOW_CAMERA_DESC ShadowCameraDesc = {};
	ShadowCameraDesc.fFar = 500.f;
	ShadowCameraDesc.fNear = 0.1f;
	ShadowCameraDesc.fFov = XMConvertToRadians(120.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_ShadowCamera"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &ShadowCameraDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TorchLamp"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("TEST_TORUCH"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlayLevel::ADD_EnviornmentLayer(const _wstring& LayerName)
{
	CGameObject::GAMEOBJECT_DESC Desc = {};
	Desc.vScale = { 1.f, 1.f, 1.f };
	Desc.vPosition = { -1500.f, -64.f, 500.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_SmallGrass"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	Desc.vPosition = { -1000.f, 14.f, 500.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_Flower"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	Desc.vPosition = { -1300.f, -64.f, 500.f };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_Default_Grass"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlayLevel::ADD_WorkAbleLayer(const _wstring& LayerName)
{
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
	Desc.vPosition = { 427.137f, -208.607f, -253.069f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlayLevel::ADD_PellLayer(const _wstring& LayerName)
{
	CPellBase::PELL_BASE_DESC Desc = {};

	Desc.bIsPellData = false;
	auto pTerrainManager = CTerrainManager::GetInstance();

	CChunk* pChunk = nullptr;
	pChunk = pTerrainManager->Find_ChunkFromTag(TEXT("SheepBalField"));
	size_t iNumCells = pChunk->GetChunckNavigation()->GetNumCells();

	for (_uInt i = 0; i < 10; ++i)
	{
		wsprintf(Desc.ObjectTag, TEXT("Drorong"));
		Desc.vScale = { 1.f, 1.f, 1.f };
		size_t CellIndex = m_pGameInstance->Random(0, iNumCells);

		Desc.vPosition = pChunk->GetChunckNavigation()->CellCenterPos(CellIndex);
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Drorong"),
			ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
			return E_FAIL;
	}

#pragma region PinkCat
	pChunk = pTerrainManager->Find_ChunkFromTag(TEXT("PinkCatField"));
	iNumCells = pChunk->GetChunckNavigation()->GetNumCells();

	for (_uInt i = 0; i < 10; ++i)
	{
		wsprintf(Desc.ObjectTag, TEXT("Bed Cat"));
		Desc.vScale = { 1.f, 1.f, 1.f };
		size_t CellIndex = m_pGameInstance->Random(0, iNumCells);
	
		Desc.vPosition = pChunk->GetChunckNavigation()->CellCenterPos(CellIndex);
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BedCat"),
			ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
			return E_FAIL;
	}
#pragma endregion
	
#pragma region ElectricPanda
	/*pChunk = pTerrainManager->Find_ChunkFromTag(TEXT("SheepBalField"));
	vChunkCenter = pChunk->GetTransform()->GetPosition();
	vChunkScale = pChunk->GetTransform()->GetScale();
	for (_uInt i = 0; i < 10; ++i)
	{
		wsprintf(Desc.ObjectTag, TEXT("ElectricPanda"));
		_float fHalfScaleX = vChunkScale.x * 0.5f;
		_float fHalfScaleZ = vChunkScale.x * 0.5f;
		Desc.vPosition = { m_pGameInstance->Random(vChunkCenter.x - fHalfScaleX, vChunkCenter.x + fHalfScaleX), 1.f,
						   m_pGameInstance->Random(vChunkCenter.z - fHalfScaleZ, vChunkCenter.z + fHalfScaleZ) };
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricPanda"),
			ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
			return E_FAIL;
	}*/
#pragma endregion

#pragma region Boss
	pChunk = pTerrainManager->Find_ChunkFromTag(TEXT("BossField"));
	iNumCells = pChunk->GetChunckNavigation()->GetNumCells();
	for (_uInt i = 0; i < 1; ++i)
	{
		wsprintf(Desc.ObjectTag, TEXT("Grass Mommoth"));
		size_t CellIndex = m_pGameInstance->Random(0, iNumCells);

		Desc.vPosition = pChunk->GetChunckNavigation()->CellCenterPos(CellIndex);
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GrassMommoth"),
			ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
			return E_FAIL;
	}
#pragma endregion


	
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

HRESULT CGamePlayLevel::ADD_NpcLayer(const _wstring& LayerName)
{
	CGameObject::GAMEOBJECT_DESC Desc = {};
	wsprintf(Desc.ObjectTag, TEXT("People_1"));

	Desc.vScale = { 1.f, 1.f, 1.f };
	Desc.vPosition = { m_pGameInstance->Random(100, 110.f), 1.f, m_pGameInstance->Random(100, 110.f) };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Female_People_1"),
		ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Desc)))
		return E_FAIL;

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

HRESULT CGamePlayLevel::LoadMainArea()
{
	// 먼저 네비 메시를 추가한다.
	CChunk::CHUNK_DESC ChunkDesc = {};
	list<SAVE_LEVEL_DESC> pObjectDesc = {};
	ReadMapFile("../Bin/Resources/DataFile/Map/Home/GamePlay_Layer_Chunk.txt", pObjectDesc);
	auto pListData = pObjectDesc.begin();
	ChunkDesc.vScale = pListData->vScale;
	ChunkDesc.vRotation = pListData->vRotation;
	ChunkDesc.vPosition = pListData->vPosition;
	ChunkDesc.NavigationFilePath = "../Bin/Resources/DataFile/Map/Home/NaviMesh.dat";
	CTerrainManager::GetInstance()->ADD_Chunk(TEXT("MainArea"), &ChunkDesc);
	auto DefualtMap = m_pGameInstance->GetAllObejctToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Terrian"))->front();
	CTerrainManager::GetInstance()->UpdateChunk(XMLoadFloat4x4(&DefualtMap->GetTransform()->GetWorldMat()));

	pObjectDesc.clear();
	ReadMapFile("../Bin/Resources/DataFile/Map/Home/GamePlay_Layer_FastTravel.txt", pObjectDesc);
	pListData = pObjectDesc.begin();
	CFastTravelObject::GAMEOBJECT_DESC FastTravelDesc = {};
	FastTravelDesc.vScale = pListData->vScale;
	FastTravelDesc.vRotation = pListData->vRotation;
	FastTravelDesc.vPosition = pListData->vPosition;
	CFastTravelObject* pFastTravel = static_cast<CFastTravelObject *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FastTravel"), &FastTravelDesc));

	WCHAR szLayerName[260] = {};
	CStringHelper::ConvertUTFToWide(pListData->LayerName, szLayerName);
	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), szLayerName, pFastTravel);
	CTerrainManager::GetInstance()->ADD_FastTravel(TEXT("MainArea"), pFastTravel);

	// 맵에 깔려있는 기본 오브젝트
	if (FAILED(LoadObject("../Bin/Resources/DataFile/Map/Home/GamePlay_Layer_Enviornment.txt", TEXT("Enviornment"))))
		return E_FAIL;

	// 인스턴싱된 Prob 오브젝트
	if (FAILED(LoadObject("../Bin/Resources/DataFile/Map/Home/GamePlay_Layer_Instance_Env.txt", TEXT("Instance_Env"))))
		return E_FAIL;

	// 마을에 상주하는 NPC 데이터
	if (FAILED(LoadObject("../Bin/Resources/DataFile/Map/Home/GamePlay_Layer_Npc.txt", TEXT("Npc"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlayLevel::LoadPalArea()
{
	auto pTerrainManager = CTerrainManager::GetInstance();
	CFastTravelObject* pFastTravel = nullptr;

#pragma region Dororong Field
	// 먼저 네비 메시를 추가한다.
	CChunk::CHUNK_DESC ChunkDesc = {};
	list<SAVE_LEVEL_DESC> pObjectDesc = {};
	ReadMapFile("../Bin/Resources/DataFile/Map/SheepBalStage/GamePlay_Layer_Chunk.txt", pObjectDesc);
	auto pListData = pObjectDesc.begin();

	ChunkDesc.vScale = pListData->vScale;
	ChunkDesc.vRotation = pListData->vRotation;
	ChunkDesc.vPosition = pListData->vPosition;
	ChunkDesc.NavigationFilePath = "../Bin/Resources/DataFile/Map/SheepBalStage/NaviMesh.dat";
	pTerrainManager->ADD_Chunk(TEXT("SheepBalField"), &ChunkDesc);

	auto DefualtMap = m_pGameInstance->GetAllObejctToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Terrian"))->front();
	CTerrainManager::GetInstance()->Find_ChunkFromTag(TEXT("SheepBalField"))->GetChunckNavigation()->Update(XMLoadFloat4x4(&DefualtMap->GetTransform()->GetWorldMat()));

	pObjectDesc.clear();
	ReadMapFile("../Bin/Resources/DataFile/Map/SheepBalStage/GamePlay_Layer_FastTravel.txt", pObjectDesc);
	pListData = pObjectDesc.begin();
	CFastTravelObject::GAMEOBJECT_DESC FastTravelDesc = {};
	FastTravelDesc.vScale = pListData->vScale;
	FastTravelDesc.vRotation = pListData->vRotation;
	FastTravelDesc.vPosition = pListData->vPosition;
	pFastTravel = static_cast<CFastTravelObject*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FastTravel"), &FastTravelDesc));

	WCHAR szLayerName[260] = {};
	CStringHelper::ConvertUTFToWide(pListData->LayerName, szLayerName);
	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), szLayerName, pFastTravel);
	pTerrainManager->ADD_FastTravel(TEXT("SheepBalField"), pFastTravel);

	
	// 맵에 깔려있는 기본 오브젝트
	if (FAILED(LoadObject("../Bin/Resources/DataFile/Map/SheepBalStage/GamePlay_Layer_Enviornment.txt", TEXT("Enviornment"))))
		return E_FAIL;

	// 인스턴싱된 Prob 오브젝트
	if (FAILED(LoadObject("../Bin/Resources/DataFile/Map/SheepBalStage/GamePlay_Layer_Instance_Env.txt", TEXT("Instance_Env"))))
		return E_FAIL;

	// 작업가능한ㄷ Prob 오브젝트
	if (FAILED(LoadEnvObject("../Bin/Resources/DataFile/Map/SheepBalStage/GamePlay_Layer_WorkAble_Rock.txt", TEXT("Rock"))))
		return E_FAIL;

#pragma endregion

#pragma region BedCat Field
	// 먼저 네비 메시를 추가한다.
	pObjectDesc.clear();
	ReadMapFile("../Bin/Resources/DataFile/Map/PinkCatStage/GamePlay_Layer_Chunk.txt", pObjectDesc);
	pListData = pObjectDesc.begin();

	ChunkDesc.vScale = pListData->vScale;
	ChunkDesc.vRotation = pListData->vRotation;
	ChunkDesc.vPosition = pListData->vPosition;
	ChunkDesc.NavigationFilePath = "../Bin/Resources/DataFile/Map/PinkCatStage/NaviMesh.dat";
	CTerrainManager::GetInstance()->ADD_Chunk(TEXT("PinkCatField"), &ChunkDesc);
	CTerrainManager::GetInstance()->Find_ChunkFromTag(TEXT("PinkCatField"))->GetChunckNavigation()->Update(XMLoadFloat4x4(&DefualtMap->GetTransform()->GetWorldMat()));

	pObjectDesc.clear();
	ReadMapFile("../Bin/Resources/DataFile/Map/PinkCatStage/GamePlay_Layer_FastTravel.txt", pObjectDesc);
	pListData = pObjectDesc.begin();
	FastTravelDesc.vScale = pListData->vScale;
	FastTravelDesc.vRotation = pListData->vRotation;
	FastTravelDesc.vPosition = pListData->vPosition;
	pFastTravel = static_cast<CFastTravelObject*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FastTravel"), &FastTravelDesc));
	
	CStringHelper::ConvertUTFToWide(pListData->LayerName, szLayerName);
	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), szLayerName, pFastTravel);
	pTerrainManager->ADD_FastTravel(TEXT("PinkCatField"), pFastTravel);

	// 맵에 깔려있는 기본 오브젝트
	//if (FAILED(LoadObject("../Bin/Resources/DataFile/Map/PinkCatStage/GamePlay_Layer_Enviornment.txt", TEXT("Enviornment"))))
	//	return E_FAIL;

	//// 인스턴싱된 Prob 오브젝트
	//if (FAILED(LoadObject("../Bin/Resources/DataFile/Map/PinkCatStage/GamePlay_Layer_Instance_Env.txt", TEXT("Instance_Env"))))
	//	return E_FAIL;

	// 작업가능한 녀석들 상주하는 NPC 데이터
	if (FAILED(LoadEnvObject("../Bin/Resources/DataFile/Map/PinkCatStage/GamePlay_Layer_WorkAlbe_Tree.txt", TEXT("Tree"))))
		return E_FAIL;
#pragma endregion

#pragma region Boss Field
	// 먼저 네비 메시를 추가한다.
	pObjectDesc.clear();
	ReadMapFile("../Bin/Resources/DataFile/Map/BossMap/GamePlay_Layer_Chunk.txt", pObjectDesc);
	pListData = pObjectDesc.begin();

	// 맵에 깔려있는 기본 오브젝트
	if (FAILED(LoadObject("../Bin/Resources/DataFile/Map/BossMap/GamePlay_Layer_Enviornment.txt", TEXT("Enviornment"))))
		return E_FAIL;

	ChunkDesc.vScale = pListData->vScale;
	ChunkDesc.vRotation = pListData->vRotation;
	ChunkDesc.vPosition = pListData->vPosition;
	ChunkDesc.NavigationFilePath = "../Bin/Resources/DataFile/Map/BossMap/NaviMesh.dat";
	CTerrainManager::GetInstance()->ADD_Chunk(TEXT("BossField"), &ChunkDesc);
	auto BossMap = m_pGameInstance->GetAllObejctToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("GamePlay_Layer_Terrain_Boss"))->front();
	CTerrainManager::GetInstance()->Find_ChunkFromTag(TEXT("BossField"))->GetChunckNavigation()->Update(XMLoadFloat4x4(&DefualtMap->GetTransform()->GetWorldMat()));

	pObjectDesc.clear();
	ReadMapFile("../Bin/Resources/DataFile/Map/BossMap/GamePlay_Layer_FastTravel.txt", pObjectDesc);
	pListData = pObjectDesc.begin();
	FastTravelDesc.vScale = pListData->vScale;
	FastTravelDesc.vRotation = pListData->vRotation;
	FastTravelDesc.vPosition = pListData->vPosition;
	pFastTravel = static_cast<CFastTravelObject*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FastTravel"), &FastTravelDesc));

	CStringHelper::ConvertUTFToWide(pListData->LayerName, szLayerName);
	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), szLayerName, pFastTravel);
	pTerrainManager->ADD_FastTravel(TEXT("BossField"), pFastTravel);

	

	//// 인스턴싱된 Prob 오브젝트
	//if (FAILED(LoadObject("../Bin/Resources/DataFile/Map/PinkCatStage/GamePlay_Layer_Instance_Env.txt", TEXT("Instance_Env"))))
	//	return E_FAIL;

	//// 작업가능한 녀석들 상주하는 NPC 데이터
	//if (FAILED(LoadEnvObject("../Bin/Resources/DataFile/Map/BossMap/GamePlay_Layer_WorkAlbe_Tree.txt", TEXT("Tree"))))
	//	return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CGamePlayLevel::LoadObject(const char* szFilePath, const WCHAR* ObjectTag)
{
	list<SAVE_LEVEL_DESC> LoadList = {};
	ReadMapFile(szFilePath, LoadList);

	WCHAR szPrototypeName[MAX_PATH] = {};
	WCHAR szLayerName[MAX_PATH] = {};
	auto pGameInstance = CGameInstance::GetInstance();
	_uInt ObejctCnt = {};
	for (auto& iter : LoadList)
	{
		CGameObject::GAMEOBJECT_DESC Desc = {};
		wsprintf(Desc.ObjectTag, TEXT("%s%d"), ObjectTag, ObejctCnt);
		Desc.vScale = iter.vScale;
		Desc.vRotation = { iter.vRotation.x,
						   iter.vRotation.y,
						   iter.vRotation.z };
		Desc.vPosition = iter.vPosition;

		CStringHelper::ConvertUTFToWide(iter.PrototypeName, szPrototypeName);
		CStringHelper::ConvertUTFToWide(iter.LayerName, szLayerName);

		pGameInstance->Add_GameObject_ToLayer(iter.iPrototypeLevelID, szPrototypeName, iter.iSaveLevelID, szLayerName, &Desc);
		ObejctCnt++;
	}
	return S_OK;
}

HRESULT CGamePlayLevel::LoadEnvObject(const char* szFilePath, const WCHAR* ObjectTag)
{
	list<SAVE_LEVEL_DESC> LoadList = {};
	ReadMapFile(szFilePath, LoadList);

	WCHAR szPrototypeName[MAX_PATH] = {};
	WCHAR szLayerName[MAX_PATH] = {};
	auto pGameInstance = CGameInstance::GetInstance();
	_uInt ObejctCnt = {};
	for (auto& iter : LoadList)
	{
		CEnviormnent::ENVIORMNENT_DESC Desc = {};
		wsprintf(Desc.ObjectTag, TEXT("%s%d"), ObjectTag, ObejctCnt);
		Desc.iModelIndex = iter.eType;
		Desc.vScale = iter.vScale;
		Desc.vRotation = { iter.vRotation.x,
						   iter.vRotation.y,
						   iter.vRotation.z };
		Desc.vPosition = iter.vPosition;

		CStringHelper::ConvertUTFToWide(iter.PrototypeName, szPrototypeName);
		CStringHelper::ConvertUTFToWide(iter.LayerName, szLayerName);

		pGameInstance->Add_GameObject_ToLayer(iter.iPrototypeLevelID, szPrototypeName, iter.iSaveLevelID, szLayerName, &Desc);
		ObejctCnt++;
	}
	return S_OK;
}

void CGamePlayLevel::ReadMapFile(const char* szFilePath, list<SAVE_LEVEL_DESC>& pOut)
{
	//파일 입출력 열어서 저장
	ios_base::openmode flag;

	flag = ios::in;

	ifstream file(szFilePath, flag);
	if (file.is_open())
	{
		_uInt iLoadObjectCnt = {};
		file >> iLoadObjectCnt;
		for (_uInt i = 0; i < iLoadObjectCnt; ++i)
		{
			SAVE_LEVEL_DESC LoadData;
			file >> LoadData.eType;
			file >> LoadData.iPrototypeLevelID;
			file >> LoadData.iSaveLevelID;
			file >> LoadData.PrototypeName;
			file >> LoadData.LayerName;
			file >> LoadData.PrototypeIndex;
			file >> LoadData.vScale.x >> LoadData.vScale.y >> LoadData.vScale.z;
			file >> LoadData.vRotation.x >> LoadData.vRotation.y >> LoadData.vRotation.z;
			file >> LoadData.vPosition.x >> LoadData.vPosition.y >> LoadData.vPosition.z;
			pOut.push_back(LoadData);
		}
	}
	file.close();
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
