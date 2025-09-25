#ifdef _DEBUG
#include "DebugApp.h"
#endif // _DEBUG
#include "MainApp.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "LoadingLevel.h"
#include "HeadUpDisplay.h"

#pragma region Manager
#include "ItemManager.h"
#include "PlayerManager.h"
#include "TerrainManager.h"
#include "PellManager.h"
#include "PellSkillManager.h"
#pragma endregion



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

	if (FAILED(SetUp_InGameFont()))
		return E_FAIL;


#ifdef _DEBUG
	if (FAILED(SetUp_DebugWindow()))
		return E_FAIL;
#endif // _DEBUG


	//// 와이어프레임 RasterizerState 생성
	//D3D11_RASTERIZER_DESC desc = {};
	//desc.FillMode = D3D11_FILL_WIREFRAME; // 와이어프레임 모드
	//desc.CullMode = D3D11_CULL_BACK;      // 백페이스 컬링
	//desc.DepthClipEnable = TRUE;

	//ID3D11RasterizerState* pWireframeRS = nullptr;
	//m_pGraphic_Device->CreateRasterizerState(&desc, &pWireframeRS);

	//// 적용
	//m_pDevice_Context->RSSetState(pWireframeRS);

	return S_OK;
}

void CMainApp::Run(_float fDeletaTime)
{
#ifdef _DEBUG
	m_pGameInstance->Set_RenderResource(0);
#endif // _DEBUG
	Update(fDeletaTime);
	Render();

#ifdef _DEBUG
	if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_5))
		m_pGameInstance->Set_RenderResource(0);
	else
		m_pGameInstance->Set_RenderResource(1);
	m_pDebugApp->Run(fDeletaTime);
#endif // _DEBUG
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

#pragma region BUTTON FRAME
	/* GamePlay_InGameMenu_ButtonFrame_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_ButtonFrame"),
		CTexture::Create(m_pGraphic_Device, m_pDevice_Context, TEXT("../Bin/Resources/Textures/UI/InGameUI/Frame/T_prt_pal_serect_focus.png"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region Shader
	/* Mouse Shader */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_TEX::Elements, VTX_TEX::iNumElements, TEXT("../Bin/ShaderFiles/TestShader.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mouse"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_TEX::Elements, VTX_TEX::iNumElements, TEXT("../Bin/ShaderFiles/MouseShader.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VTXNorTex"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_NORTEX::Elements, VTX_NORTEX::iNumElements, TEXT("../Bin/ShaderFiles/VTX_NorTex.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_TEX::Elements, VTX_TEX::iNumElements, TEXT("../Bin/ShaderFiles/ProgressBar.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_MutiplyBlend"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_TEX::Elements, VTX_TEX::iNumElements, TEXT("../Bin/ShaderFiles/MutipleBlend.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_MESH::Elements, VTX_MESH::iNumElements, TEXT("../Bin/ShaderFiles/VTX_Mesh.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_ANIM_MESH::Elements, VTX_ANIM_MESH::iNumElements, TEXT("../Bin/ShaderFiles/VTX_AnimMesh.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_SkyMesh"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_MESH::Elements, VTX_MESH::iNumElements, TEXT("../Bin/ShaderFiles/SkyMeshDefault.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_WorldUI"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_MESH::Elements, VTX_MESH::iNumElements, TEXT("../Bin/ShaderFiles/WorldUI.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_InstanceMesh"),
		CShader::Create(m_pGraphic_Device, m_pDevice_Context, VTX_MODEL_INSTANCE_DESC::Elements, VTX_MODEL_INSTANCE_DESC::iNumElements, TEXT("../Bin/ShaderFiles/VTX_InstnaceMesh.hlsl")))))
		return E_FAIL;
#pragma endregion

#pragma region VIBuffer
	/* VIBuffer  RECT  Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphic_Device, m_pDevice_Context))))
		return E_FAIL;
#pragma endregion

#pragma region  Component
	/* Transform Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphic_Device, m_pDevice_Context))))
		return E_FAIL;

	/* Animator Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animation"), CSpriteAnimation::Create(m_pGraphic_Device, m_pDevice_Context))))
		return E_FAIL;

#pragma region Collision
	/* BoxCollision Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionBox"), CBoxCollision::Create(m_pGraphic_Device, m_pDevice_Context))))
		return E_FAIL;

	/* SphereCollision Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionSphere"), CSphereCollision::Create(m_pGraphic_Device, m_pDevice_Context))))
		return E_FAIL;

	/* OBBCollision Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), COBBCollision::Create(m_pGraphic_Device, m_pDevice_Context))))
		return E_FAIL;
#pragma endregion

#pragma region Font Component
	/* OBBCollision Component */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), CFontComponent::Create(m_pGraphic_Device, m_pDevice_Context))))
		return E_FAIL;
#pragma endregion


#pragma endregion

	

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

HRESULT CMainApp::SetUp_InGameFont()
{
	if (FAILED(m_pGameInstance->Add_Font(TEXT("HanSanFont_16"), TEXT("../Bin/Resources/Fonts/Spoqa_Han_Sans_Neo_Medium16.spritefont"))))
		return E_FAIL;

	if( FAILED(m_pGameInstance->Add_Font(TEXT("HanSanFont_20"), TEXT("../Bin/Resources/Fonts/Spoqa_Han_Sans_Neo_Medium20.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("HanSanFont_24"), TEXT("../Bin/Resources/Fonts/Spoqa_Han_Sans_Neo_Medium24.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("HanSanFont_32"), TEXT("../Bin/Resources/Fonts/Spoqa_Han_Sans_Neo_Medium32.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("HanSanFont_50"), TEXT("../Bin/Resources/Fonts/Spoqa_Han_Sans_Neo_Medium50.spritefont"))))
		return E_FAIL;
		
	if (FAILED(m_pGameInstance->Add_Font(TEXT("HanSanFont_56"), TEXT("../Bin/Resources/Fonts/Spoqa_Han_Sans_Neo_Medium56.spritefont"))))
		return E_FAIL;

	return S_OK;
}

#ifdef _DEBUG
HRESULT CMainApp::SetUp_DebugWindow()
{
	m_pDebugApp = CDebugApp::Create(m_pGraphic_Device, m_pDevice_Context);;

	return S_OK;
}
#endif

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

#ifdef _DEBUG
	Safe_Release(m_pDebugApp);
#endif // __DEBUG
	
	CPlayerManager::DestroyInstance();
	CItemManager::DestroyInstance();
	CPellManager::DestroyInstance();
	CTerrainManager::DestroyInstance();
	CPellSkillManager::DestroyInstance();

	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);
}
