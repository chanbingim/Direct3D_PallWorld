#include "MainApp.h"
#include "GameInstance.h"

#include "LoadingLevel.h"
#include "HeadUpDisplay.h"
#include "GameData_Manager.h"

CMainApp::CMainApp() : m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize_MainApp()
{
	if (FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if (FAILED(SetUp_StaticComponents()))
		return E_FAIL;

	if (FAILED(SetUp_StartLevel(LEVEL::LOGO)))
		return E_FAIL;

	if (FAILED(SetUp_CameraSetting()))
		return E_FAIL;

	if (FAILED(SetUp_MouseTexture()))
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
	auto LoadingLevel = CLoadingLevel::Create(m_pGraphic_Device, m_pDevice_Context, LEVEL::LOADING, eLevelID);
	if (FAILED(m_pGameInstance->Change_Level(LoadingLevel)))
		return E_FAIL;

	LoadingLevel->Initialize();
	return S_OK;
}

HRESULT CMainApp::SetUp_StaticComponents()
{
#pragma region Mouse Com
	/* Mouse Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Mouse_Texture"), CTexture::Create(m_pGraphic_Device, m_pDevice_Context,
				TEXT("../Bin/Resources/Textures/UI/MouseCursor/GUI_MouseCursor_Default.png"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region Loading BackGround Texture
	/* Loding_BackGround_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingBackGround"),
		CTexture::Create(m_pGraphic_Device, m_pDevice_Context, TEXT("../Bin/Resources/Textures/UI/Loading/Black.png"), 1))))
		return E_FAIL;

	/* Loding_Icon_Front_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingIconFront"),
		CTexture::Create(m_pGraphic_Device, m_pDevice_Context, TEXT("../Bin/Resources/Textures/UI/Loading/Icon/IconFront.png"), 1))))
		return E_FAIL;

	/* Loding_Icon_Back_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingIconBack"),
		CTexture::Create(m_pGraphic_Device, m_pDevice_Context, TEXT("../Bin/Resources/Textures/UI/Loading/Icon/IconBack.png"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region LOGO TEXTURE
	/* Logo_Text_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_GameLogo"),
		CTexture::Create(m_pGraphic_Device, m_pDevice_Context, TEXT("../Bin/Resources/Textures/UI/MainLogo/GameLogo/T_Palworld_Logo_Small_White.png"), 1))))
		return E_FAIL;

	/* Logo_Text_Shadow_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_GameLogo_Shadow"),
		CTexture::Create(m_pGraphic_Device, m_pDevice_Context, TEXT("../Bin/Resources/Textures/UI/MainLogo/GameLogo/T_Title_logo_shadow.png"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region Shader
	/* Mouse Shader */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_TEX::VertexDesc, VTX_TEX::iNumElements, TEXT("../Bin/ShaderFiles/TestShader.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mouse"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_TEX::VertexDesc, VTX_TEX::iNumElements, TEXT("../Bin/ShaderFiles/MouseShader.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXNorTex"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_NORTEX::VertexDesc, VTX_NORTEX::iNumElements, TEXT("../Bin/ShaderFiles/VTX_NorTex.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_TEX::VertexDesc, VTX_TEX::iNumElements, TEXT("../Bin/ShaderFiles/ProgressBar.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_MutiplyBlend"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_TEX::VertexDesc, VTX_TEX::iNumElements, TEXT("../Bin/ShaderFiles/MutipleBlend.hlsl")))))
		return E_FAIL;
#pragma endregion

	/*VIBuffer  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphic_Device, m_pDevice_Context))))
		return E_FAIL;

	/* Transform Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphic_Device, m_pDevice_Context))))
		return E_FAIL;

	/* Transform Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animation"), CSpriteAnimation::Create(m_pGraphic_Device, m_pDevice_Context))))
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

HRESULT CMainApp::SetUp_MouseTexture()
{
	if (FAILED(m_pGameInstance->SetMouseTexture(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Mouse_Texture"), TEXT("Texture_Com"), nullptr,
		TEXT("Prototype_Component_Shader_Mouse"), TEXT("Shader_Com"), nullptr,
		TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), nullptr)))
		return E_FAIL;

	ShowCursor(false);
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
	CGameData_Manager::DestroyInstance();

	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);
}
