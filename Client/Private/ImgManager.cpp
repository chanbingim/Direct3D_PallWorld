#include "ImgManager.h"

HRESULT CImgManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(pDevice, pContext);

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    // 전체화면 윈도우 설정
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;

    io.DisplaySize = { g_iWinSizeX, g_iWinSizeY };
    //m_ImGuiIo = io;

    // 창 위치를 (0,0)으로 설정
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGuiWindowFlags ImGuiWindowFlag = ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoScrollbar |
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoDecoration |
                    ImGuiWindowFlags_NoNav |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_MenuBar;

    // 패딩과 테두리를 고려해 창 스타일 비활성화
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    return S_OK;
}

void CImgManager::Update(_float fDeletaTime)
{
    // 윈도우 메시지 처리 등...
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 예: 데모 윈도우
    ImGui::ShowDemoWindow();
}

void CImgManager::Render()
{
    ImGui::Render();
    // 여기서 백버퍼 클리어, 다른 렌더 등 수행
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

CImgManager* CImgManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CImgManager* pInstance = new CImgManager;
    if (FAILED(pInstance->Initialize(pDevice, pContext)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : IMG MANAGER");
    }
    return pInstance;
}

void CImgManager::Free()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
