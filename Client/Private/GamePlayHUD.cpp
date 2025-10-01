#include "GamePlayHUD.h"

#include "GameInstance.h"
#include "BackGround.h"

#include "InGameMenu.h"
#include "CreateMenu.h"
#include "SelectUI.h"

CGamePlayHUD::CGamePlayHUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CHeadUpDisplay(pDevice, pContext)
{
}

HRESULT CGamePlayHUD::Initialize()
{
	if (FAILED(ADD_UserInterface()))
		return E_FAIL;

	m_pGameInstance->ShowInGameMouse(VISIBILITY::VISIBLE);
	return S_OK;
}

void CGamePlayHUD::Update(_float fDeletaTime)
{
	UIKeyInput();
	Change_ViewUI();

	__super::Update(fDeletaTime);
}

void CGamePlayHUD::SetVisibleSelectUI(VISIBILITY eVisible)
{
	m_pSelectUI->SetVisibility(eVisible);
}

void CGamePlayHUD::ResetCreatePopUp()
{
	m_pCreateMenu->SetActive(false);
	FoucusInUserInterface(false);
}

CSelectUI* CGamePlayHUD::GetSelectUI()
{
	return m_pSelectUI;
}

HRESULT CGamePlayHUD::ADD_UserInterface()
{
	//여기서 계층 만들어서 세팅
	CUserInterface::GAMEOBJECT_DESC Desc;
	ZeroMemory(&Desc, sizeof(CUserInterface::GAMEOBJECT_DESC));

	Desc.vScale = { g_iWinSizeX * 0.3f, g_iWinSizeY * 0.2f , 1.f };
	Desc.vPosition = { g_iHalfWinSizeX * 0.2f,  g_iWinSizeY * 0.8f, 0.f };

	if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_PlayerInfo"), TEXT("InGame_PlayerInfoUI"), &Desc)))
		return E_FAIL;

	Desc.vScale = { g_iWinSizeX * 0.5f, 40.f , 1.f };
	Desc.vPosition = { g_iHalfWinSizeX, 20.f, 0.f };

	if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Compass"), TEXT("InGame_Compass"), &Desc)))
		return E_FAIL;

	Desc.vScale = { g_iWinSizeX * 0.8f , g_iWinSizeY * 0.85f , 1.f };
	Desc.vPosition = { g_iHalfWinSizeX, g_iWinSizeY * 0.55f, 0.f };

	if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_InGmaeMenu_UI"), TEXT("InGame_Menu"), &Desc, (CUserInterface**)&m_pInGameMenu)))
		return E_FAIL;

	Desc.vScale = { g_iWinSizeY * 0.1f , g_iWinSizeY * 0.1f , 1.f };
	Desc.vPosition = { g_iHalfWinSizeX, g_iHalfWinSizeY, 0.f };

	if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_CrossHair_UI"), TEXT("CrossHair"), &Desc, (CUserInterface**)&m_pCrossHair)))
		return E_FAIL;

	Desc.vScale = { g_iWinSizeY , g_iWinSizeY , 1.f };
	Desc.vPosition = { g_iHalfWinSizeX, g_iHalfWinSizeY, 0.f };
	if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Tech_Create"), TEXT("Create_Menu"), &Desc, (CUserInterface**)&m_pCreateMenu)))
		return E_FAIL;

	Desc.vScale = { 50.f, 20.f, 0.f };
	m_pSelectUI = CSelectUI::Create(m_pDevice, m_pContext);
	m_pSelectUI->SetZOrder(100.f);
	if (FAILED(m_pSelectUI->Initialize(&Desc)))
		return E_FAIL;

	m_pSelectUI->SetVisibility(VISIBILITY::HIDDEN);
	m_pUserInterfaceMap.emplace(TEXT("SelectUI"), m_pSelectUI);

	return S_OK;
}

void CGamePlayHUD::UIKeyInput()
{
	if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_I))
	{
		m_pCreateMenu->SetActive(false);
		if (false == m_pInGameMenu->IsActive())
		{
			m_pInGameMenu->SetActive(true);
			FoucusInUserInterface(true);
		}
		else
		{
			m_pInGameMenu->SetActive(false);
			FoucusInUserInterface(false);
		}
	}

	if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_B))
	{
		m_pInGameMenu->SetActive(false);
		if (false == m_pCreateMenu->IsActive())
		{
			m_pCreateMenu->SetActive(true);
			FoucusInUserInterface(true);
		}
		else
		{
			m_pCreateMenu->SetActive(false);
			FoucusInUserInterface(false);
		}
	}
}

void CGamePlayHUD::Change_ViewUI()
{
	if (m_Visible)
	{
		if (m_pInGameMenu->IsActive())
		{
			if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 0))
			{
				if (!PtInRect(&m_pSelectUI->GetRectSize(), m_pGameInstance->GetMousePoint()))
					SetVisibleSelectUI(VISIBILITY::HIDDEN);
			}
		}
		else
			SetVisibleSelectUI(VISIBILITY::HIDDEN);
	}
	else
	{
		m_pGameInstance->SetMousePosition({ g_iHalfWinSizeX, g_iHalfWinSizeY, 0.f });
		if(g_GameWindowFocus)
			SetCursorPos(g_iHalfWinSizeX, g_iHalfWinSizeY);
	}
}

void CGamePlayHUD::FoucusInUserInterface(_bool bFlag)
{
	if (bFlag)
	{
		m_Visible = true;
		m_pGameInstance->SetGamePause(true);
		m_pGameInstance->ShowInGameMouse(VISIBILITY::VISIBLE);
	}
	else
	{
		m_Visible = false;
		m_pGameInstance->SetGamePause(false);
		m_pGameInstance->ShowInGameMouse(VISIBILITY::HIDDEN);
	}
}

void CGamePlayHUD::ALL_PopUp_UnActive()
{
	m_pInGameMenu->SetActive(false);
	m_pCreateMenu->SetActive(false);
}

CGamePlayHUD* CGamePlayHUD::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGamePlayHUD* pGameplayHUD = new CGamePlayHUD(pDevice, pContext);
	if (nullptr == pGameplayHUD)
		MSG_BOX("CREATE FAIL : GAMEPLAY HUD");

	return pGameplayHUD;
}

void CGamePlayHUD::Free()
{
	__super::Free();
}
