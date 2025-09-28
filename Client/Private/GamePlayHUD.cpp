#include "GamePlayHUD.h"

#include "GameInstance.h"
#include "BackGround.h"

#include "InGameMenu.h"

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
	__super::Update(fDeletaTime);
	if (!m_pInGameMenu->IsActive())
	{
		//m_pGameInstance->SetMousePosition({ g_iHalfWinSizeX, g_iHalfWinSizeY, 0.f });
	/*	if(g_GameWindowFocus)
			SetCursorPos(g_iHalfWinSizeX, g_iHalfWinSizeY);*/
	}
		
	if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_I))
	{
		_bool bIsFlag = !m_pInGameMenu->IsActive();
		if(bIsFlag)
			m_pGameInstance->ShowInGameMouse(VISIBILITY::VISIBLE);
		else
			m_pGameInstance->ShowInGameMouse(VISIBILITY::HIDDEN);

		m_pInGameMenu->SetActive(bIsFlag);
		m_pGameInstance->SetGamePause(bIsFlag);
	}
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

	return S_OK;
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
