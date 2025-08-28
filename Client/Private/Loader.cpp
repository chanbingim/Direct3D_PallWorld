#include "Loader.h"

#include "GameInstance.h"

#pragma region LOGO Header
#include "Logo_Menu.h"
#include "PellLogo.h"

#pragma region LOGO BUTTON
#include "Logo_GameStart_But.h"
#include "Logo_GameEdit_But.h"
#include "Logo_GameQuit_But.h"
#pragma endregion

#pragma endregion

#pragma region GamePlay Header
#include "Camera.h"
#include "Terrian.h"

#pragma region PELL
#include "BedCat.h"
#include "Dororong.h"
#include "ElectPanda.h"
#include "Herorong.h"
#include "FlowerRabbit.h"
#include "Hedgehog.h"
#include "Frog.h"
#include "Yeti.h"
#pragma endregion

#pragma region UI
#include "Player_Interface.h"

#include "HealthBar.h"
#include "GuardBar.h"
#include "HealthIcon.h"
#include "HungerIcon.h"

#include "Compass.h"
#include "CompassPoint.h"

#include "BattlePell.h"
#include "PellInfo.h"
#include "PellInfoFrame.h"
#include "TypeIcon.h"

#include "InGameMenu.h"
#include "Player.h"
#pragma endregion

#pragma endregion


CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	m_pDevice(pDevice),
	m_pContext(pContext),
	m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* 세마포어, 뮤텍스, 크리티컬섹션 */

	/* 임계영역(힙, 데이터, 코드)에 접근하기위한 키를 생성한다. */
	InitializeCriticalSection(&m_CriticalSection);

	/* 실제 로딩을 수행하기위한 스레드를 생성한다. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	if (FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	EnterCriticalSection(&m_CriticalSection);
	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL::LOGO:
		hr = Loading_For_Logo();
		break;
	case LEVEL::GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

void CLoader::Output()
{
	SetWindowText(g_hWnd, m_strMessage.c_str());
}

HRESULT CLoader::Loading_For_Logo()
{
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
	/* Logo_Title_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MainLogo/T_title_BG.png"), 1))))
		return E_FAIL;
	
	/* Logo_GameStartButtonTexture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_LMenu_GameStart_But"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MainLogo/Button/GameStart/Gs_But%d.png"), 2))))
		return E_FAIL;

	/* Logo_GameEditButtonTexture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_LMenu_GameEdit_But"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MainLogo/Button/GameEdit/Ge_But%d.png"), 2))))
		return E_FAIL;

	/* Logo_GameQuitButtonTexture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_LMenu_GameQuit_But"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MainLogo/Button/GameQuit/Gq_But%d.png"), 2))))
		return E_FAIL;

	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

	/* GAME_OBJECT_LOGO_MENU_BACKGROUND */
	if(FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LogoMenu"), CLogo_Menu::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_LOGO_MENU_BACKGROUND_TEXT */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_PellLogo"), CPellLogo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_LOGO_MENU_BACKGROUND_LMenu_GameStart_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LMenu_But_GameStart"), CLogo_GameStart_But::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_LOGO_MENU_BACKGROUND_LMenu_GameEdit_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LMenu_But_GameEdit"), CLogo_GameEdit_But::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_LOGO_MENU_BACKGROUND_LMenu_GameQuit_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_LMenu_But_GameQuit"), CLogo_GameQuit_But::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	m_strMessage = TEXT("로딩이 완료되었습니다..");

	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");

#pragma region Terrian_Texture
	/* GamePlay_Terrian_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrian"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Map/Tile/Tile%d.dds"), 2))))
		return E_FAIL;
#pragma endregion

#pragma region PLAYER_INFO_TEXTURE
	/* GamePlay_PlayerInfo_bar_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_HeatlhBar_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Progressbar/T_gauge_HP%d.png"), 2))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Icon_Hunger_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_HungerIcon_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Icon/T_gauge_icon_hunger.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Icon_Health_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_HeatlhIcon_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Icon/T_gauge_icon_HP.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Guard_Bar_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_GuardBar_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Progressbar/T_gauge_shield_base_0.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Compass_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Compass_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Compass/T_prt_compass_base%d.png"), 2))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Compass_Point_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Compass_Point_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Compass/T_prt_compass_icon_base.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Battle_Pell_UI_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_Pell_UI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/SelectPell/T_gauge_pal_skill.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Battle_PellInfoFrame_UI_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_Pell_TypeUI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/PellType/T_Icon_element_s_%d.png"), 9))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Battle_PellInfo_BG_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellInfo_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/SelectPell/InfoBackGround/InfoBackground.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Battle_Pell_TypeUI_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellFrame_UI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/SelectPell/Frame/T_prt_icon_circle_f_%d.png"), 2))))
		return E_FAIL;

#pragma endregion

#pragma region IN GAME MENU TEXTURE
	/* GamePlay_InGameMenu_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/GameMenu/InGameBackGround.png"), 1))))
		return E_FAIL;

	/* GamePlay_InGameMenu_Category_UI_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_Category_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/GameMenu/Category/Category_base%d.png"), 3))))
		return E_FAIL;

	/* GamePlay_InGameMenu_OptionButton_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_OptionBut"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/GameMenu/Option/OptionButBase%d.png"), 2))))
		return E_FAIL;
#pragma endregion

#pragma region ModelTexture
	/* GamePlay_Model_Fiona_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Fiona"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Fiona/fiona_D.png"), 1))))
		return E_FAIL;
#pragma endregion

	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

#pragma region Models

#pragma region Player
	/* VIBuffer  MESH  Component */
	_matrix PreModelMat = XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Fiona_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma region PELL
	/* VIBuffer  BedCat MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_BedCat_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM,"../Bin/Resources/Models/BedCat/PinkCat.fbx"))))
		return E_FAIL;

	///* VIBuffer  Dororong MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Drorong_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Dororong/SheepBall.fbx"))))
	//	return E_FAIL;

	///* VIBuffer  Electric Panda MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_ElectricPanda_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/ElectricPanda/Panda.fbx"))))
	//	return E_FAIL;

	///* VIBuffer  Herorong MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Herorong_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Herorong/Herorong.fbx"))))
	//	return E_FAIL;

	///* VIBuffer  Electric Herorong MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Electric_Herorong_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Herorong/EletricHerorong.fbx"))))
	//	return E_FAIL;
 
	///* VIBuffer  FlowerRabbit MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_FlowerRabbit_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/FlowerRabbit/FlowerRabbit.fbx"))))
	//	return E_FAIL;

	///* VIBuffer  Hedgehog MESH  Component */
	////if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Hedgehog_Mesh"),
	////	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Hedgehog/Panda.fbx"))))
	////	return E_FAIL;

	///* VIBuffer  Frog MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Frog_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Frog/Frog.fbx"))))
	//	return E_FAIL;

	///* VIBuffer  Yeti MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Yeti_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Yeti/GrassYeti.fbx"))))
	//	return E_FAIL;
#pragma endregion

#pragma endregion


	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

#pragma region Terrian_Com
	/* 2 x 2  Terrian */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrian2x2"), CVIBuffer_Terrian::Create(m_pDevice, m_pContext, 2))))
		return E_FAIL;

	/* 4 x 4  Terrian */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrian4x4"), CVIBuffer_Terrian::Create(m_pDevice, m_pContext, 4))))
		return E_FAIL;

	/* 8 x 8  Terrian */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrian8x8"), CVIBuffer_Terrian::Create(m_pDevice, m_pContext, 8))))
		return E_FAIL;

	/* 16 x 16  Terrian */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrian16x16"), CVIBuffer_Terrian::Create(m_pDevice, m_pContext, 16))))
		return E_FAIL;

	/* 32 x 32  Terrian */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrian32x32"), CVIBuffer_Terrian::Create(m_pDevice, m_pContext, 32))))
		return E_FAIL;

	/* 64 x 64  Terrian */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrian64x64"), CVIBuffer_Terrian::Create(m_pDevice, m_pContext, 64))))
		return E_FAIL;

	/* 128 x 128  Terrian */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrian128x128"), CVIBuffer_Terrian::Create(m_pDevice, m_pContext, 128))))
		return E_FAIL;

	/* 256 x 256  Terrian */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrian256x256"), CVIBuffer_Terrian::Create(m_pDevice, m_pContext, 256))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrian"), CVIBuffer_Terrian::Create(m_pDevice, m_pContext,
		TEXT("../Bin/Resources/Textures/Map/HeightMap/Height.bmp")))))
		return E_FAIL;
#pragma endregion

#pragma region OBJECT
	/* GAME_OBJECT_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera"), CCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Terrian */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrian"), CTerrian::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region PELL
	/* GAME_OBJECT_BedCat */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BedCat"), CBedCat::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Dororong */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Drorong"), CDororong::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_ElectricPanda */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricPanda"), CElectPanda::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Herorong */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Herorong"), CHerorong::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_FlowerRabbit */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FlowerRabbit"), CFlowerRabbit::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Hedgehog */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Hedgehog"), CHedgehog::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Frog */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Frog"), CFrog::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Yeti */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Yeti"), CYeti::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

	
#pragma endregion

#pragma region PLAYER_INFO
	/* GAME_OBJECT_PlayerInfoUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_PlayerInfo"), CPlayer_Interface::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_HealthBarUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Health_Bar"), CHealthBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_HungerIconUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Hunger_Icon"), CHungerIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_HealthIconUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Health_Icon"), CHealthIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_GuardBarUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Guard_Bar"), CGuardBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Compass_Point */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Compass_Point"), CCompassPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Compass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Compass"), CCompass::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Battle_Pell_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Battle_Pell_UI"), CBattlePell::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Battle_Pell_InfoUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Battle_PellInfo_UI"), CPellInfo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Battle_Pell_InfoFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Battle_PellFrame_UI"), CPellInfoFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Battle_Pell_TypeUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Battle_Pell_TypeUI"), CTypeIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region IN GAME MENU UI
	/* GAME_OBJECT_InGameMenu_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_InGmaeMenu_UI"), CInGameMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
	m_strMessage = TEXT("로딩이 완료되었습니다..");

	m_isFinished = true;
	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pLoader = new CLoader(pDevice, pContext);
	if (FAILED(pLoader->Initialize(eNextLevelID)))
	{
		Safe_Release(pLoader);
		MSG_BOX("CREATE FAIL : LOADER");
	}

	return pLoader;
}

void CLoader::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
