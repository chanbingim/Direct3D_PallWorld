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

	/* Logo_Text_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_GameLogo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MainLogo/GameLogo/T_Palworld_Logo_Small_White.png"), 1))))
		return E_FAIL;

	/* Logo_Text_Shadow_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_GameLogo_Shadow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/MainLogo/GameLogo/T_Title_logo_shadow.png"), 1))))
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

	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

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
