#include "GamePlayHUD.h"

#include "GameInstance.h"
#include "BackGround.h"

#include "InGameMenu.h"
#include "CreateMenu.h"
#include "DiallogUI.h"
#include "WorkBenchCreateUI.h"

#pragma region PreViewUI
#include "SelectUI.h"
#include "CreateToolTipUI.h"
#pragma endregion

CGamePlayHUD::CGamePlayHUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CHeadUpDisplay(pDevice, pContext)
{
}

HRESULT CGamePlayHUD::Initialize()
{
	if (FAILED(ADD_UserInterface()))
		return E_FAIL;

	if (FAILED(ADD_PreviewUserInterface()))
		return E_FAIL;

	UnActiveAllPopUp();
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

HRESULT CGamePlayHUD::ActivePopUpUserInterface(_uInt iID)
{
	for (auto& pPopup : m_PopupUIs)
		pPopup.second->SetVisibility(VISIBILITY::HIDDEN);

	auto pair = m_PopupUIs.find(iID);
	if (pair == m_PopupUIs.end())
		return E_FAIL;

	pair->second->SetVisibility(VISIBILITY::VISIBLE);
	FoucusInUserInterface(true);
	return S_OK;
}

HRESULT CGamePlayHUD::UnActivePopUpUserInterface(_uInt iID)
{
	auto pair = m_PopupUIs.find(iID);
	if (pair == m_PopupUIs.end())
		return E_FAIL;

	pair->second->SetVisibility(VISIBILITY::HIDDEN);
	FoucusInUserInterface(false);

	return S_OK;
}

HRESULT CGamePlayHUD::ActivePreviewUserInterface(_uInt iID)
{
	auto pair = m_PopupUIs.find(iID);
	if (pair == m_PopupUIs.end())
		return E_FAIL;

	pair->second->SetVisibility(VISIBILITY::VISIBLE);
	return S_OK;
}

HRESULT CGamePlayHUD::UnActivePreviewUserInterface(_uInt iID)
{
	auto pair = m_PreviewUIs.find(iID);
	if (pair == m_PreviewUIs.end())
		return E_FAIL;

	pair->second->SetVisibility(VISIBILITY::HIDDEN);
	return S_OK;
}

CUserInterface* CGamePlayHUD::GetPopUpUserInterface(_uInt iPopupID)
{
	auto pair = m_PopupUIs.find(iPopupID);
	if (pair == m_PopupUIs.end())
		return nullptr;

	return pair->second;
}

CUserInterface* CGamePlayHUD::GetPreViewUserInterface(_uInt iPopupID)
{
	auto pair = m_PreviewUIs.find(iPopupID);
	if (pair == m_PreviewUIs.end())
		return nullptr;

	return pair->second;
}

CSelectUI* CGamePlayHUD::GetSelectUI()
{
	return m_pSelectUI;
}

void CGamePlayHUD::UnActiveAllPopUp()
{
	for (auto& pair : m_PopupUIs)
		pair.second->SetVisibility(VISIBILITY::HIDDEN);

	FoucusInUserInterface(false);
}

void CGamePlayHUD::UnActiveAllPreView()
{
	for (auto& pair : m_PreviewUIs)
		pair.second->SetVisibility(VISIBILITY::HIDDEN);
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
	Safe_AddRef(m_pInGameMenu);
	m_PopupUIs.emplace(0, m_pInGameMenu);

	Desc.vScale = { g_iWinSizeY * 0.1f , g_iWinSizeY * 0.1f , 1.f };
	Desc.vPosition = { g_iHalfWinSizeX, g_iHalfWinSizeY, 0.f };

	if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_CrossHair_UI"), TEXT("CrossHair"), &Desc)))
		return E_FAIL;

	Desc.vScale = { g_iWinSizeY , g_iWinSizeY , 1.f };
	Desc.vPosition = { g_iHalfWinSizeX, g_iHalfWinSizeY, 0.f };
	if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Tech_Create"), TEXT("Create_Menu"), &Desc, (CUserInterface**)&m_pCreateMenu)))
		return E_FAIL;

	Safe_AddRef(m_pCreateMenu);
	m_PopupUIs.emplace(1, m_pCreateMenu);

#pragma region Archtecture Create UI
	Desc.vScale = { g_iWinSizeY * 0.7f , g_iWinSizeY * 0.8f , 1.f };
	Desc.vPosition = { g_iHalfWinSizeX, g_iHalfWinSizeY, 0.f };
	if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WorkbenchCreateUI"), TEXT("WorkBench_Create_Menu"), &Desc, (CUserInterface**)&m_pWorkBenchCreateMenu)))
		return E_FAIL;

	Safe_AddRef(m_pWorkBenchCreateMenu);
	m_PopupUIs.emplace(2, m_pWorkBenchCreateMenu);
#pragma endregion

#pragma region DialLog
	CDiallogUI* pDiallog = nullptr;

	/* DialLog UI*/
	Desc.vScale = { g_iWinSizeY * 0.8f , 200.f , 1.f };
	Desc.vPosition = { g_iHalfWinSizeX, g_iWinSizeY - Desc.vScale.y * 0.5f, 0.f };
	if (FAILED(__super::Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DialLogUI"), TEXT("DialLog_UI"), &Desc, (CUserInterface**)&pDiallog)))
		return E_FAIL;

	Safe_AddRef(pDiallog);
	m_PopupUIs.emplace(3, pDiallog);
#pragma endregion
	
	return S_OK;
}

HRESULT CGamePlayHUD::ADD_PreviewUserInterface()
{
	CUserInterface::GAMEOBJECT_DESC Desc = {};

#pragma region SelectView
	Desc.vScale = { 50.f, 20.f, 0.f };
	m_pSelectUI = CSelectUI::Create(m_pDevice, m_pContext);
	m_pSelectUI->SetZOrder(100.f);
	if (FAILED(m_pSelectUI->Initialize(&Desc)))
		return E_FAIL;

	m_pSelectUI->SetVisibility(VISIBILITY::HIDDEN);
	m_PreviewUIs.emplace(0, m_pSelectUI);
	m_pUserInterfaceMap.emplace(TEXT("SelectUI"), m_pSelectUI);

#pragma endregion

#pragma region Create Tool tip
	Desc.vScale = { 200.f, 120.f, 0.f };
	m_pCreateToolTip = CCreateToolTipUI::Create(m_pDevice, m_pContext);
	m_pCreateToolTip->SetZOrder(100.f);
	if (FAILED(m_pCreateToolTip->Initialize(&Desc)))
		return E_FAIL;

	m_pCreateToolTip->SetVisibility(VISIBILITY::HIDDEN);
	m_PreviewUIs.emplace(1, m_pCreateToolTip);
	m_pUserInterfaceMap.emplace(TEXT("Create_ToolTip"), m_pCreateToolTip);
#pragma endregion

	return S_OK;
}

void CGamePlayHUD::UIKeyInput()
{
	if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_I))
	{
		if (VISIBILITY::HIDDEN == m_pInGameMenu->GetVisibility())
		{
			ActivePopUpUserInterface(0);
		}
		else
		{
			UnActivePopUpUserInterface(0);
		}
	}

	if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_B))
	{
		if (VISIBILITY::HIDDEN == m_pCreateMenu->GetVisibility())
		{
			ActivePopUpUserInterface(1);
		}
		else
		{
			UnActivePopUpUserInterface(1);
		}
	}

	if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_C))
	{
		if (VISIBILITY::HIDDEN == m_PopupUIs.find(3)->second->GetVisibility())
		{
			ActivePopUpUserInterface(3);
			
			auto pDiallog = static_cast<CDiallogUI *>(m_PopupUIs.find(3)->second);
			pDiallog->SetDiallogText(TEXT("안녕 하세요 반갑습니다. 테스트 테스트"));
		
		}
		else
		{
			UnActivePopUpUserInterface(3);
		}
	}
}

void CGamePlayHUD::Change_ViewUI()
{
	if (m_Visible)
	{
		if (VISIBILITY::VISIBLE == m_pInGameMenu->GetVisibility())
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

	for (auto pair : m_PopupUIs)
		Safe_Release(pair.second);
}
