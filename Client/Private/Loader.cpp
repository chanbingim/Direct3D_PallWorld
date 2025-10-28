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
#include "Terrain.h"

#pragma region Map
#include "DefaultMap.h"
#include "BossMap.h"
#include "CubeSky.h"
#pragma endregion

#pragma region PLAYER
#include "PlayerPartData.h"
#include "PlayerBody.h"
#include "PlayerWeaponSlot.h"
#include "PlayerSlotArchitecture.h"
#include "WeaponQuickSlot.h"
#include "ItemQuickSlot.h"
#pragma endregion

#pragma region NPC

#pragma region Female
#include "FemalePeopleBody_1.h"
#include "FemalePeople_1.h"
#pragma endregion

#pragma endregion

#pragma region SKY_BOX
#include "SkyBox.h"
#pragma endregion

#pragma region PELL
#include "BedCat.h"
#include "BedCatBody.h"

#include "Dororong.h"
#include "DororongBody.h"

#include "ElectPanda.h"
#include "ElectricPandaBody.h"

#include "Herorong.h"
#include "HerorongBody.h"

#include "FlowerRabbit.h"
#include "FlowerLabbitBody.h"

#include "Hedgehog.h"
#include "HedgehogBody.h"

#include "Frog.h"
#include "FrogBody.h"

#include "Yeti.h"
#include "YetiBody.h"

#include "GreenMommoth.h"
#include "GrassMommothBody.h"
#pragma endregion

#pragma region PreViewModel
#include "PreivewModelBody.h"
#include "PreviewModelWeapon.h"
#pragma endregion

#pragma region ENVIORNMENT
#include "RockObject.h"
#include "TreeObject.h"
#include "PellRock.h"

#pragma region Instance Prob
#include "SmallGrass.h"
#include "DefaultGrass.h"
#include "GrassBush.h"
#include "GrassLeave.h"
#include "Flower.h"
#pragma endregion

#include "ViliageCurchObject.h"
#include "ViliageHouseObject.h"
#include "ViliageMarketObject.h"
#include "FastTravelObject.h"
#pragma endregion

#pragma region FUNRNITURE
#include "BedFurniture.h"
#include "TableFurniture.h"
#include "TouchLamp.h"
#pragma endregion

#pragma region ITEM
#include "ItemObject.h"
#pragma endregion

#pragma region Components
#include "Recovery.h"
#include "DropComponent.h"
#include "AiSenceComponent.h"
#include "WorkComponent.h"
#pragma endregion

#pragma region SKY
#include "SunLight.h"
#include "ShadowCamera.h"
#pragma endregion

#pragma region UI
#include "Player_Interface.h"
#include "AimInterface.h"

#include "BossHealthBar.h"
#include "HealthBar.h"
#include "GuardBar.h"
#include "HealthIcon.h"
#include "HungerBar.h"
#include "HungerIcon.h"

#include "Compass.h"
#include "CompassPoint.h"

#include "BattlePell.h"
#include "PellInfo.h"
#include "NeturalPellInfo.h"
#include "FastTravelUI.h"

#include "PellInfoFrame.h"
#include "TypeIcon.h"

#pragma region PalBoxUI
#include "PalBoxUserInterface.h"
#pragma endregion

#pragma region SKILL_OBJECT
#include "GrassStrom.h"
#include "Earthquake.h"
#pragma endregion

#pragma region EFFECT
#include "HitEffect.h"
#include "TrailEffect.h"
#include "ElectricBall.h"
#pragma endregion

#pragma region WORKBENCH
#include "WorkBenchCreateUI.h"
#include "CreateToolTipUI.h"
#include "WorkBenchSlotView.h"
#pragma endregion

#pragma region BASE UI
#include "TitleUI.h"
#pragma endregion

#pragma region Diallog
#include "DiallogUI.h"
#pragma endregion

#include "IngredientUI.h"

#pragma region CREATE UI
#include "CreateMenu.h"
#include "TechListViewSlot.h"
#pragma endregion

#pragma region PELL INFO
#include "PellHealthBar.h"
#include "NeturalTypeIcon.h"
#pragma endregion

#pragma region PAL SPHER INFO
#include "PalSpherUI.h"
#include "PallSpherPercentBar.h"
#pragma endregion

#pragma region ITEM_UI
#include "ItemInfoUI.h"
#pragma endregion

#pragma region Button
#include "EventButton.h"
#include "FastTravelButton.h"
#include "TechCategoryUI.h"
#pragma endregion

#include "InGameMenu.h"
#include "Player.h"
#pragma endregion

#pragma region PROJECTILE
#include "PalSpher.h"
#include "ArrowProjectile.h"
#include "EletricBullet.h"
#pragma endregion

#pragma region BUILD OBJECT
#include "PalBed.h"
#include "PalBox.h"
#include "WorkBench.h"

#pragma region WOOD
#include "DefenceWallWood.h"
#include "WoodWall.h"
#include "WoodDoor.h"
#pragma endregion

#pragma endregion

#pragma endregion

#pragma region ClientComponents
#include "CombatComponent.h"
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

#pragma region SKY
	/* GamePlay_Sun_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Sun_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Map/Sun/TempSun.dds"), 1))))
		return E_FAIL;

	/* GamePlay_Cube_Sky_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Sky_Cube"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Map/SkyBox/Sky.dds"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region FastTravelBackGround
	/* GamePlay_Pell_FastTravel_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_BackGorund_FastTravel_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/BackGround/FastTravelBackGround.png"), 1))))
		return E_FAIL;
#pragma endregion


#pragma region DEFULAT_BUTTON_TEX
	/* GamePlay_PlayerInfo_Button_Default_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Button_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/BackGround/T_prt_inventory_gauge_frame.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Button_Default_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Button_SkillBase"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/TechUI/TechBut/T_prt_pal_skill_base_01.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Button_Tech_Category_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Category_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/TechUI/TechBut/T_prt_loadout_base.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Button_Slot_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Slot_Base_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Slot/SlotBack.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Button_Tech_Slot_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Slot_Base_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Slot/T_prt_technorogy_frame_blur.png"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region MyRegion

#pragma endregion

#pragma region Select Tri Tex
	/* GamePlay_Select_Tri_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Button_Select_Tri"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/GameMenu/Category/T_prt_menutab_tri.png"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region CREATE MENU TEXTURE

#pragma region TabTex
	/* GamePlay_PlayerInfo_Button_Tech_Category_Tech_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/TechUI/TechBut/Tab/T_icon_construction_tab_0%d.png"), 10))))
		return E_FAIL;
#pragma endregion

#pragma region PreviewCircle
	/* GamePlay_PlayerInfo_Button_Tech_Menu_OutRidal_Tex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Crate_OutCircle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/TechUI/TechBut/T_prt_radial_base.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Button_Tech_Category_Tech_InRidal_Tex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Crate_InCircle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/TechUI/TechBut/T_prt_radial_base_line.png"), 1))))
		return E_FAIL;

#pragma endregion

#pragma region FONT BACKGROUND

	/* GamePlay_PlayerInfo_Button_Tech_Font_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Font_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/TechUI/TechBut/FontBackGround.png"), 1))))
		return E_FAIL;

#pragma endregion

#pragma endregion

#pragma region PalSlotTexture
	/* GamePlay_PalSlot_Texture_Inven */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_PalSlot_Inven_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Slot/PalSlot/Inven/BackGround%d.png"), 2))))
		return E_FAIL;

	/* GamePlay_PalSlot_Texture_Inven_Frame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_PalSlot_Inven_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Slot/PalSlot/Inven/T_prt_pal_base_frame.png"), 1))))
		return E_FAIL;

	/* GamePlay_PalSlot_Texture_Base */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_PalSlot_Base_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Slot/PalSlot/Base/T_prt_pal_ct_base.png"), 1))))
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
	/* GamePlay_PlayerInfo_Battle_PellTypes_UI_BackGroundTexture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_Pell_BackGroundTypeUI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/PellType/PellIconBackGround.png"), 1))))
		return E_FAIL;

	/* GamePlay_PlayerInfo_Battle_PellTypes_UI_Texture */
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

#pragma region WEAPON_UI_TEXTURE

	/* GamePlay_PlayerInfo_Battle_Pell_TypeUI_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_WeaponQuickSlot_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/WeaponUI/BackGround/T_prt_loadout_base.png"), 1))))
		return E_FAIL;

#pragma endregion

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

#pragma region PELL_INFO_TEXTURE
	/* GamePlay_Pell_Health_BackGround_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Pell_HeatlhBar_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Progressbar/T_gauge_HP0.png"), 1))))
		return E_FAIL;

	/* GamePlay_Pell_PellInfoUI_BackGround_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_PellInfo_BackGround_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/PalInfoUI/T_prt_enemy_base_line.png"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region PAL SPHER UI
	/* GamePlay_Pell_PalSpher_UI_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Pell_PalSpherUI_PercentBar_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/Progressbar/PalSpherUI/PalSpherPercentBar.png"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region Action UI Texture
	/* GamePlay_Pell_PellInfoUI_BackGround_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_ActionUI_BackGround_Tex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/InGameUI/BackGround/ActionBase.png"), 1))))
		return E_FAIL;
#pragma endregion


#pragma region CrossHair
	/* GamePlay_CrossHair_DefualtType_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_CrossHair_Defualt"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Aim/T_aim_0.png"), 2))))
		return E_FAIL;
#pragma endregion

#pragma region Models

#pragma region MAP_MODEL
	_matrix PreModelMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0, -200.f, 0);	
	/* VIBuffer  Default Map */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_DefaultMap"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/SmallMap/Map.dat"))))
		return E_FAIL;

	/* VIBuffer Boss Map */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_BossMap"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/BossMap/Map.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma region Clothes
	PreModelMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	/* VIBuffer  Old_Clothes MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_OldClothes"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Clothes/OldClothes/OldClothes.fbx", PreModelMat,
			"../Bin/Resources/Models/Clothes/OldClothes/OldColtehsRetarget.txt"))))
		return E_FAIL;

	/* VIBuffer  Clothes2 MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Clothes02"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Clothes/Clothes02/Clothes02.fbx", PreModelMat, 
			"../Bin/Resources/Models/Clothes/Clothes02/Clothes02_Mapping.txt"))))
		return E_FAIL;
#pragma endregion

#pragma region SKY_Model
	/* VIBuffer  Clothes2 MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Sky"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Sky/SM_Raid_Sky_01.dat"))))
		return E_FAIL;
#pragma endregion

#pragma region PROJECTILE
	PreModelMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	/* VIBuffer  Arrow  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Arrow"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Weapon/AnimationWeapon/Bow/Arrow/Arrow.dat", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma region Build Object
	PreModelMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	/* VIBuffer  PAL BOX  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_PalBox"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/BuildObject/PalBox/PalBox.dat"))))
		return E_FAIL;

	/* VIBuffer  PAL BED  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_PalBed"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/BuildObject/PalBed/PalBed.dat"))))
		return E_FAIL;

	/* VIBuffer  WORK BENCH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_WorkBench"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/BuildObject/WorkBench/WorkBench.dat"))))
		return E_FAIL;

#pragma region BuildObject Wood
	/* VIBuffer  Wood Wall  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_WoodWall"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/BuildObject/Wood/Wall/WoodWall.dat"))))
		return E_FAIL;

	/* VIBuffer  Wood Door  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_WoodDoor"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/BuildObject/Wood/Door/Door_Wood.dat"))))
		return E_FAIL;

	/* VIBuffer  Defence Wood Wall  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_DefenceWoodWall"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/BuildObject/Wood/DefenceWall/DefenceWall_Wood.dat"))))
		return E_FAIL;
#pragma endregion

#pragma region FURNITURE
	/* VIBuffer  TORCH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Torch_Stand"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/BuildObject/Furniture/Torch_Stand/TorchStand.dat"))))
		return E_FAIL;

	/* VIBuffer  BED PRIMITIVE  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_BedFurniture"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/BuildObject/Furniture/Bed_Primitive/BedPrimitive.dat"))))
		return E_FAIL;

	/* VIBuffer  TABLE  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Table"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/BuildObject/Furniture/Table/Table.dat"))))
		return E_FAIL;
#pragma endregion


#pragma endregion

#pragma region NPC

#pragma region FEMALE

#pragma region PEOPLE 1
	/* VIBuffer  FEMLAE PeoPle1  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Female_People_1"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/NPC/Female/People1/FemalePeople.dat", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma endregion

#pragma region WEAPON
	PreModelMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	/* VIBuffer  CatBlade MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_CatBlade"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Weapon/CatBlade/CatBlade.dat"))))
		return E_FAIL;

	///* VIBuffer  Prop MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_CatBladeProp"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Weapon/CatBlade/CatBladeProp.fbx", PreModelMat))))
	//	return E_FAIL;

	/* VIBuffer  PellSpher MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_PalSpher"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Weapon/AnimationWeapon/PalSphere/PalSpher.dat"))))
		return E_FAIL;

	/* VIBuffer  Bow MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Bow"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Weapon/AnimationWeapon/Bow/Bow.dat"))))
		return E_FAIL;

#pragma endregion

#pragma region Player
	/* VIBuffer  MESH  Component */
	PreModelMat = XMMatrixScaling(0.01f,0.01f,0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Player_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Player/Player.dat", PreModelMat, "", 2))))
		return E_FAIL;

#pragma endregion

#pragma region PELL
#pragma region PinkCat
	/* VIBuffer  BedCat MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_BedCat_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM,"../Bin/Resources/Models/Monster/BedCat/PinkCat.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma region SheepBall
	/* VIBuffer  Dororong MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Drorong_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Monster/SheepBall/SheepBall.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma region ElecPanda
	/* VIBuffer  Electric Panda MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_ElectricPanda_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Monster/ElectricPanda/ElecPanda.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma region Grass Mommoth
	/* VIBuffer  GrassMommoth MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_GrassMommoth_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Monster/GreenMommoth/GrassMommoth.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

	///* VIBuffer  Herorong MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Herorong_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Herorong/Herorong.fbx", PreModelMat))))
	//	return E_FAIL;

	///* VIBuffer  Electric Herorong MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Electric_Herorong_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Herorong/EletricHerorong.fbx", PreModelMat))))
	//	return E_FAIL;
 //
	///* VIBuffer  FlowerRabbit MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_FlowerRabbit_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/FlowerRabbit/FlowerRabbit.fbx", PreModelMat))))
	//	return E_FAIL;

	///* VIBuffer  Hedgehog MESH  Component */
	////if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Hedgehog_Mesh"),
	////	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Hedgehog/Panda.fbx"))))
	////	return E_FAIL;

	///* VIBuffer  Frog MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Frog_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Frog/Frog.fbx", PreModelMat))))
	//	return E_FAIL;

	///* VIBuffer  Yeti MESH  Component */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Yeti_Mesh"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::ANIM, "../Bin/Resources/Models/Yeti/GrassYeti.fbx", PreModelMat))))
	//	return E_FAIL;
#pragma endregion

#pragma region Clothes
/* VIBuffer  Clothes2 MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Grass"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/SmallGrass/small_Grass.fbx", PreModelMat))))
		return E_FAIL;

	/* VIBuffer  Clothes2 MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Flower"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/Flower/Flower.dat", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma region EnviornMent

#pragma region SMALL Grass InstanceBuffer
	CVIBuffer_Model_Instance::MODEL_INSTANCE_DESC InstanceModelDesc = {};
	InstanceModelDesc.iNumInstance = 3000.f;
	InstanceModelDesc.vSize = { 1.f, 1.f, 1.f };
	InstanceModelDesc.vCenter = { 0.f ,0.f, 0.f };
	InstanceModelDesc.vRange = { 200.f ,0.f, 200.f };

	InstanceModelDesc.pModelFilePath = "../Bin/Resources/Models/Enviornmenet/SmallGrass/small_Grass.fbx";
	InstanceModelDesc.PreModelMat = PreModelMat;
	InstanceModelDesc.RetargetFile = "";
	InstanceModelDesc.iLayerCount = 1;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Instance_SamllGrass"),
				CVIBuffer_Model_Instance::Create(m_pDevice, m_pContext, &InstanceModelDesc))))
				return E_FAIL;
#pragma endregion

#pragma region SMALL Grass InstanceBuffer
	InstanceModelDesc.iNumInstance = 3000.f;
	InstanceModelDesc.vSize = { 1.f, 1.f, 1.f };
	InstanceModelDesc.vCenter = { 0.f ,0.f, 0.f };
	InstanceModelDesc.vRange = { 200.f ,0.f, 200.f };

	InstanceModelDesc.pModelFilePath = "../Bin/Resources/Models/Enviornmenet/Grass/Grass.dat";
	InstanceModelDesc.PreModelMat = PreModelMat;
	InstanceModelDesc.RetargetFile = "";
	InstanceModelDesc.iLayerCount = 1;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Instance_DefaultGrass"),
		CVIBuffer_Model_Instance::Create(m_pDevice, m_pContext, &InstanceModelDesc))))
		return E_FAIL;
#pragma endregion

#pragma region FLOWER InstaceBuffer;
	InstanceModelDesc.iNumInstance = 3000.f;
	InstanceModelDesc.vSize = { 1.f, 1.f, 1.f };
	InstanceModelDesc.vCenter = { 0.f ,0.f, 0.f };
	InstanceModelDesc.vRange = { 200.f ,0.f, 200.f };

	InstanceModelDesc.pModelFilePath = "../Bin/Resources/Models/Enviornmenet/Flower/Flower.dat";
	InstanceModelDesc.PreModelMat = PreModelMat;
	InstanceModelDesc.RetargetFile = "";
	InstanceModelDesc.iLayerCount = 1;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Instance_Flower"),
		CVIBuffer_Model_Instance::Create(m_pDevice, m_pContext, &InstanceModelDesc))))
		return E_FAIL;
#pragma endregion

#pragma region Grass Bush InstanceBuffer
	InstanceModelDesc.iNumInstance = 3000.f;
	InstanceModelDesc.vSize = { 1.f, 1.f, 1.f };
	InstanceModelDesc.vCenter = { 0.f ,0.f, 0.f };
	InstanceModelDesc.vRange = { 200.f ,0.f, 200.f };

	InstanceModelDesc.pModelFilePath = "../Bin/Resources/Models/Enviornmenet/GrassBush/Grass02.fbx";
	InstanceModelDesc.PreModelMat = PreModelMat;
	InstanceModelDesc.RetargetFile = "";
	InstanceModelDesc.iLayerCount = 1;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Instance_GrassBush"),
		CVIBuffer_Model_Instance::Create(m_pDevice, m_pContext, &InstanceModelDesc))))
		return E_FAIL;
#pragma endregion

#pragma region Grass Leave InstanceBuffer
	InstanceModelDesc.iNumInstance = 3000.f;
	InstanceModelDesc.vSize = { 1.f, 1.f, 1.f };
	InstanceModelDesc.vCenter = { 0.f ,0.f, 0.f };
	InstanceModelDesc.vRange = { 200.f ,0.f, 200.f };

	InstanceModelDesc.pModelFilePath = "../Bin/Resources/Models/Enviornmenet/GrasLeave/Grass03.fbx";
	InstanceModelDesc.PreModelMat = PreModelMat;
	InstanceModelDesc.RetargetFile = "";
	InstanceModelDesc.iLayerCount = 1;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Instance_GrasLeave"),
		CVIBuffer_Model_Instance::Create(m_pDevice, m_pContext, &InstanceModelDesc))))
		return E_FAIL;
#pragma endregion


#pragma region Viliage
	/* VIBuffer  Viliage House1 Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_ViliageHouse1"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/Viliage/House/House1/House1.dat"))))
		return E_FAIL;

	/* VIBuffer Viliage Church Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_ViliageChurch"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/Viliage/Church/Church.dat"))))
		return E_FAIL;

	/* VIBuffer  Viliage Market Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_ViliageMarket"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/Viliage/Market/Market.dat"))))
		return E_FAIL;
#pragma endregion

#pragma region Fast Travel
	/* VIBuffer  Viliage Market Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_FastTravel"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/FastTravel/FastTravel.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma region ROCK
	/* EnviornMent Rock MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rock_0_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/Rock/Rock/Rock/Rock02/Rock02.fbx", PreModelMat))))
		return E_FAIL;

	/* EnviornMent Rock3 MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rock_1_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/Rock/Rock/Rock/Rock03/Rock03.fbx", PreModelMat))))
		return E_FAIL;

	/* EnviornMent Rock3 MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Rock_2_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/Rock/Rock/ClifRock/Clif01/Clif_Rock1.fbx", PreModelMat))))
		return E_FAIL;

#pragma region PalJium
	/* EnviornMent PalJium MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_PellJium_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/Rock/Ore/Paljium.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma region TREE
	/* EnviornMent Tree0 MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Tree0_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/Tree/Tree03/b03.fbx", PreModelMat))))
		return E_FAIL;

	/* EnviornMent Tree1 MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Tree1_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Enviornmenet/Tree/Tree04/b04.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma endregion

#pragma region Shader
	
#pragma endregion

#pragma region ITEM

#pragma region ROCK
	/* ITEM Rock MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Item_Rock_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Item/Rock/ItemRock.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma region Wood
	/* ITEM Rock MESH  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Item_Wood_Mesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, "../Bin/Resources/Models/Item/Wood/Wood.fbx", PreModelMat))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma region Component
	/* GamePlay_Component_CombatComponent */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Combat"),
		CCombatComponent::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GamePlay_Component_ChaseComponent*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Chase"),
		CChaseComponent::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GamePlay_Component_RecoveryComponent */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("GamePlay_Component_Recovery"), CRecovery::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GamePlay_Component_DropComponent */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_Component_Drop"), CDropComponent::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GamePlay_Component_Ai_Secne_Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_Component_AiSence"), CAiSenceComponent::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GamePlay_Component_Work_Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_Component_Work"), CWorkComponent::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region SKY
	/* Sun */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_SunLight"), CSunLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Shadow_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_ShadowCamera"), CShadowCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region OBJECT
	/* GAME_OBJECT_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera"), CCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Terrian */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrian"), CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region MAP_OBJECT
	/* Map Defualt */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DefualtMap"), CDefaultMap::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Map Boss */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BossMap"), CBossMap::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region SKY
	/* GAME_OBJECT_SKY_BOX */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SkyBox"), CSkyBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_SKY_BOX */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_CubeSkyBox"), CCubeSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region PELL

#pragma region BedCat
	/* GAME_OBJECT_BedCat */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BedCat"), CBedCat::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_BedCatBody */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BedCat_Body_Default"), CBedCatBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Drorong
	/* GAME_OBJECT_Dororong */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Drorong"), CDororong::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_DrorongBody */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Drorong_Body_Default"), CDororongBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region ElectricPanda
	/* GAME_OBJECT_ElectricPanda */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricPanda"), CElectPanda::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_ElectricPanda */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricPanda_Body_Default"), CElectricPandaBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion
//
//#pragma region Herorong
//	/* GAME_OBJECT_Herorong */
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Herorong"), CHerorong::Create(m_pDevice, m_pContext))))
//		return E_FAIL;
//
//	/* GAME_OBJECT_Herorong */
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Herorong_Body_Default"), CHerorongBody::Create(m_pDevice, m_pContext))))
//		return E_FAIL;
//#pragma endregion
//
//#pragma region FlowerRabbit
//	/* GAME_OBJECT_FlowerRabbit */
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FlowerRabbit"), CFlowerRabbit::Create(m_pDevice, m_pContext))))
//		return E_FAIL;
//
//	/* GAME_OBJECT_FlowerRabbitBody */
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FlowerRabbit_Body_Default"), CFlowerLabbitBody::Create(m_pDevice, m_pContext))))
//		return E_FAIL;
//#pragma endregion
//
//#pragma region Hedgehog
//	/* GAME_OBJECT_Hedgehog */
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Hedgehog"), CHedgehog::Create(m_pDevice, m_pContext))))
//		return E_FAIL;
//
//	/* GAME_OBJECT_HedgehogBody */
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Hedgehog_Body_Default"), CHedgehogBody::Create(m_pDevice, m_pContext))))
//		return E_FAIL;
//#pragma endregion
//
//#pragma region Frog
//	/* GAME_OBJECT_Frog */
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Frog"), CFrog::Create(m_pDevice, m_pContext))))
//		return E_FAIL;
//
//	/* GAME_OBJECT_FrogBody */
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Frog_Body_Default"), CFrogBody::Create(m_pDevice, m_pContext))))
//		return E_FAIL;
//#pragma endregion
//
//#pragma region Yeti
//	/* GAME_OBJECT_Yeti */
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Yeti"), CYeti::Create(m_pDevice, m_pContext))))
//		return E_FAIL;
//
//	/* GAME_OBJECT_YetiBody */
//	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Yeti_Body_Default"), CYetiBody::Create(m_pDevice, m_pContext))))
//		return E_FAIL;
//#pragma endregion

#pragma region Grass Mommoth
	/* GAME_OBJECT_Grass Mommoth */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GrassMommoth"), CGreenMommoth::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Grass Mommoth Body */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GrassMommoth_Body_Default"), CGrassMommothBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma region ENVIORNMENT

#pragma region ROCK
	/* GAME_OBJECT_Rock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rock"), CRockObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Tree */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Tree"), CTreeObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_PellJium */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PelJium"), CPellRock::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Instance Prob
#pragma region Small Grass
	/* GAME_OBJECT_SmallGrass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_SmallGrass"),
		CSmallGrass::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Flower
	/* GAME_OBJECT_SmallGrass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_Flower"),
		CFlower::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Default Grass
	/* GAME_OBJECT_DefaultGrass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_Default_Grass"),
		CDefaultGrass::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Grass Bush
	/* GAME_OBJECT_DefaultGrass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_Grass_Bush"),
		CGrassBush::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Grass Leave
	/* GAME_OBJECT_DefaultGrass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_Grass_Leave"),
		CGrassLeave::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma region Viliage Prob
	/* GAME_OBJECT_Viliage_House */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_Viliage_House"),
		CViliageHouseObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Viliage_Curch */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_Viliage_Church"),
		CViliageCurchObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Viliage_Market */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Environment_Viliage_Market"),
		CViliageMarketObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma endregion

	
#pragma endregion

#pragma region PLAYER
	/* GAME_OBJECT_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Animatior */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_PartData_Default"), CPlayerPartData::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Clothes */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Body_Default"), CPlayerBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Weapon Slot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_WeaponSot"), CPlayerWeaponSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Item Slot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_BackItemSlot"), CPlayerItemSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Architecture Slot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Slot_Architecture"), CPlayerSlotArchitecture::Create(m_pDevice, m_pContext))))
		return E_FAIL;
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

	/* GAME_OBJECT_HungerBarUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Hunger_Bar"), CHungerBar::Create(m_pDevice, m_pContext))))
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

#pragma region WEAPON UI

	/* GAME_OBJECT_Weapon_QuickSlot_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Weapon_QuickSlot"), CWeaponQuickSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region Item QuickSlot UI
	/* GAME_OBJECT_Item_QuickSlot_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Item_QuickSlot"), CItemQuickSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma region PELL_INFO_UI
	/* GAME_OBJECT_PlayerInfoUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Netrual_Pell_Health_Bar"), CPellHealthBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Pell_TypeUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Netrual_Pell_Type_UI"), CNeturalTypeIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region PAL SPHER INFO
	/* GAME_OBJECT_PAlSpher_Info_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PalSpherInfo_UI"), CPalSpherUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_PAlSpher_Info_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PalSpherInfo_PercentBar"), CPallSpherPercentBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region IN GAME MENU UI
	/* GAME_OBJECT_InGameMenu_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_InGmaeMenu_UI"), CInGameMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_CrossHair_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_CrossHair_UI"), CAimInterface::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Pell_Info_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PellInfo_UI"), CNeturalPellInfo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region ITEM INFO UI
	/* GAME_OBJECT_InGameMenu_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Info_UI"), CItemInfoUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region EVENT UI
	/* GAME_OBJECT_InGameMenu_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Event_Button"), CEventButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Animation_Button_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Event_FastTravelButton"), CFastTravelButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_InGameMenu_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Tech_Category"), CTechCategoryUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Boss Health Bar
	/* GAME_OBJECT_Boss_Health_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Boss_HealthBar"), CBossHealthBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region CREATE UI
	/* GAME_OBJECT_CREATE_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Tech_Create"), CCreateMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_TECH_LIST_VIEW_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Tech_List_View_Slot"), CTechListViewSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Projectile

	/* GAME_OBJECT_PalSpher */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PalSpher"), CPalSpher::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_ArrowProjecttile */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ArrowProjectile"), CArrowProjectile::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_ElectricBall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricBall"), CElectricBall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region BUILD OBJECT
	/* GAME_OBJECT_PalBox */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PalBox"), CPalBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_PalBed */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PalBed"), CPalBed::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_WorkBench */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WorkBench"), CWorkBench::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region Furniture
	/* GAME_OBJECT_BedFurniture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BedFurniture"), CBedFurniture::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_TorchLamp */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TorchLamp"), CTouchLamp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_TableFurniture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TableFurniture"), CTableFurniture::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion


#pragma region WOOD
	/* GAME_OBJECT_DefenceWoodWall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DefenceWoodWall"), CDefenceWallWood::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_WoodWall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WoodWall"), CWoodWall::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_WoodDoor */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WoodDoor"), CWoodDoor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_FastTravel */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FastTravel"), CFastTravelObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma region ITEM OBJECT
	/* GAME_OBJECT_ItemObject */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ItemObject"), CItemObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region BASE UI
	
	/* GAME_OBJECT_Title_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Base_Title_UI"), CTitleUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region NPC

#pragma region FEMALE

#pragma region People 1
	/* GAME_OBJECT_Female_People_1_Body */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Female_People_Body_1"), CFemalePeopleBody_1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Female_People_1_Object */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Female_People_1"), CFemalePeople_1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion


#pragma endregion

#pragma endregion

#pragma region WORKBENCH UI
	/* GAME_OBJECT_WorkBench_Create_Menu */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WorkbenchCreateUI"), CWorkBenchCreateUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_WorkBench_ToolTip */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Workbench_ToolTip_UI"), CCreateToolTipUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_WorkBench_SlotView */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Workbench_SlotView"), CWorkBenchSlotView::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region DialLogUI
	/* GAME_OBJECT_DialLog */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DialLogUI"), CDiallogUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region PalBoxUI
	/* GAME_OBJECT_PalBoxUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PalBoxUserInterface"), CPalBoxUserInterface::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region FstTravelUI
	/* GAME_OBJECT_FastTravel */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FastTravel_UI"), CFastTravelUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Effect
#pragma region SKILL_OBEJCT
	/* GAME_OBJECT_EarthQuake */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Pal_Skill_Earthquake"), CEarthquake::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_GrassStome */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Pal_Skill_GrassStrom"), CGrassStrom::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Hit_Effect
	/* GAME_OBJECT_Hit_Effect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Hit_Effect_Defalut"), CHitEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Trail
	/* GAME_OBJECT_Trail_Effect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Trail_Defalut"), CTrailComponent::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion
#pragma endregion

#pragma region PreViewModel
	/* GAME_OBJECT_Hit_Effect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PreviewModelBody"), CPreivewModelBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* GAME_OBJECT_Hit_Effect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PreviewModelWeapon"), CPreviewModelWeapon::Create(m_pDevice, m_pContext))))
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
