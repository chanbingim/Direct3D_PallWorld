#include "MainApp.h"
#include "GameInstance.h"

#include "LoadingLevel.h"
#include "HeadUpDisplay.h"

CMainApp::CMainApp() : m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize_MainApp()
{
	if (FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if (FAILED(SetUp_StartLevel(LEVEL::LOGO)))
		return E_FAIL;

	if (FAILED(SetUp_StaticComponents()))
		return E_FAIL;

	if (FAILED(SetUp_CameraSetting()))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Run(_float fDeletaTime)
{
	Update(fDeletaTime);
	Render();
}

HRESULT CMainApp::SetUp_DefaultSetting()
{
	CGameInstance::GAMEINSTANCE_DESC GameInstanceDesc;

	GameInstanceDesc.EngineSetting.hWnd = g_hWnd;
	GameInstanceDesc.EngineSetting.Instance = g_hInstance;
	GameInstanceDesc.EngineSetting.bIsWindowed = true;
	GameInstanceDesc.EngineSetting.iNumLevels = ENUM_CLASS(LEVEL::END);
	GameInstanceDesc.EngineSetting.iWinSizeX = g_iWinSizeX;
	GameInstanceDesc.EngineSetting.iWinSizeY = g_iWinSizeY;

	GameInstanceDesc.ppDevice = &m_pGraphic_Device;
	GameInstanceDesc.ppContext = &m_pDevice_Context;

	if (FAILED(m_pGameInstance->Initialize_Engine(&GameInstanceDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::SetUp_StartLevel(LEVEL eLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(CLoadingLevel::Create(m_pGraphic_Device, m_pDevice_Context, LEVEL::LOADING, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::SetUp_StaticComponents()
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphic_Device, m_pDevice_Context))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::SetUp_CameraSetting()
{
	D3D11_RASTERIZER_DESC RasterizerDesc; 
	ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	RasterizerDesc.CullMode = D3D11_CULL_NONE;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthClipEnable = true;

	if(FAILED(m_pGraphic_Device->CreateRasterizerState(&RasterizerDesc, &m_pRasterState)))
		return E_FAIL;

	
	m_pDevice_Context->RSSetState(m_pRasterState);

	return S_OK;
}

void CMainApp::Update(_float fDeletaTime)
{
	m_pGameInstance->Update_Engine(fDeletaTime);
}

void CMainApp::Render()
{
	m_pGameInstance->Render_Begin(m_fDefalutColor);

	m_pGameInstance->Draw();

	m_pGameInstance->Render_End();
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp();
	if (FAILED(pMainApp->Initialize_MainApp()))
	{
		Safe_Release(pMainApp);
		MSG_BOX("CREATE FAIL : MAIN APP");
	}
	return pMainApp;
}

void CMainApp::Free()
{
	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pRasterState);

	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);
}
