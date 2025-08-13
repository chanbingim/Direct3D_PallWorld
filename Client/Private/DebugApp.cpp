#ifdef _DEBUG
#include "DebugApp.h"

#include "GameInstance.h"
#include "ImgManager.h"

CDebugApp::CDebugApp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext),
    m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CDebugApp::Initialize_MainApp()
{
    if (FAILED(SetUp_DefaultSetting()))
        return E_FAIL;

    if (FAILED(SetUp_ImgManager()))
        return E_FAIL;

    return S_OK;
}

void CDebugApp::Run(_float fDeletaTime)
{
    Update(fDeletaTime);
    Render();
}

HRESULT CDebugApp::SetUp_DefaultSetting()
{
    ENGINE_DESC     EngineDesc;

    EngineDesc.hWnd = g_hWnd_Debug;
    EngineDesc.Instance = g_hInstance_Debug;
    EngineDesc.bIsWindowed = true;
    EngineDesc.iNumLevels = ENUM_CLASS(LEVEL::END);
    EngineDesc.iWinSizeX = g_iWinSizeX;
    EngineDesc.iWinSizeY = g_iWinSizeY;

    m_pGameInstance->ADD_Window(EngineDesc);
    return S_OK;
}

HRESULT CDebugApp::SetUp_ImgManager()
{
    m_pImgManager = CImgManager::Create(m_pDevice, m_pContext);
    return S_OK;
}

void CDebugApp::Update(_float fDeletaTime)
{
    m_pImgManager->Update(fDeletaTime);
}

void CDebugApp::Render()
{
    _float Color[] = {0.f, 0.f, 1.f, 1.f};
    m_pGameInstance->Render_Begin(Color);

    m_pImgManager->Render();

    m_pGameInstance->Render_End();
}

CDebugApp* CDebugApp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDebugApp* pInstance = new CDebugApp(pDevice, pContext);
    if (FAILED(pInstance->Initialize_MainApp()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : DEBUG APP");
    }

    return pInstance;
}

void CDebugApp::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);

    Safe_Release(m_pImgManager);
}
#endif