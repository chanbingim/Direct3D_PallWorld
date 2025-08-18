#ifdef  _DEBUG

#include "ImgManager.h"

#include "GameObject.h"

#pragma region IMG_UI_HEADER
#include "IMG_Profiler.h"
#include "IMG_Hierarchy.h"
#include "IMG_Inspector.h"
#include "IMG_LandScape.h"
#include "IMG_Viewport.h"
#include "IMG_Content.h"
#pragma endregion


IMPLEMENT_SINGLETON(CImgManager);

HRESULT CImgManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    if (FAILED(Default_Setting(pDevice, pContext)))
        return E_FAIL;

    if (FAILED(Setting_Img_UI()))
        return E_FAIL;

    return S_OK;
}

void CImgManager::Update(_float fDeletaTime)
{
    // 윈도우 메시지 처리 등...
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    if (ImGui::Begin("Edit Window", nullptr, m_ImGuiWindowFlags))
    {
        CreateDockSpace();
        DarwMenuBar();

        for (auto& pair : m_ImgUIMap)
            pair.second->Update(fDeletaTime);

        ImGui::End();
    }
}

void CImgManager::Render()
{
    ImGui::Render();

    // 여기서 백버퍼 클리어, 다른 렌더 등 수행
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CImgManager::ADD_SelectObject(CGameObject* SelectObj)
{
    auto iter = find(m_SelectList.begin(), m_SelectList.end(), SelectObj);
    if (iter == m_SelectList.end())
    {
        m_SelectList.push_back(SelectObj);
        Safe_AddRef(SelectObj);
    }
}

void CImgManager::Remove_SelectObject(CGameObject* SelectObj)
{
    auto iter = find(m_SelectList.begin(), m_SelectList.end(), SelectObj);
    if (iter != m_SelectList.end())
    {
        Safe_Release(*iter);
        m_SelectList.erase(iter);
    }
}

const list<CGameObject*>* CImgManager::GetSelectObjects()
{
    return &m_SelectList;
}

void CImgManager::ClearAllSelectObjects()
{
    for (auto& iter : m_SelectList)
        Safe_Release(iter);

    m_SelectList.clear();
}

HRESULT CImgManager::ADD_IMG_UserInterface(const _wstring szTag, CImgUIBase* pUserInterface)
{
    auto pair = m_ImgUIMap.find(szTag);

    if (pair == m_ImgUIMap.end())
        m_ImgUIMap.emplace(szTag, pUserInterface);
    else
    {
        MSG_BOX("Already Created : UI");
        return E_FAIL;
    }

    return S_OK;
}

CImgUIBase* CImgManager::Find_ImgUserInterface(const WCHAR* szUITag)
{
    auto pair = m_ImgUIMap.find(szUITag);

    if (pair == m_ImgUIMap.end())
        return nullptr;
    return pair->second;
}

HRESULT CImgManager::Default_Setting(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplWin32_Init(g_hWnd_Debug);
    ImGui_ImplDX11_Init(pDevice, pContext);

    //멀티 윈도우 기준이함수를 호출해야 잘 랜더링됨
    ImGui_ImplDX11_CreateDeviceObjects();

    // 전체화면 윈도우 설정
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
    io.DisplaySize = { g_iWinSizeX, g_iWinSizeY };

    ImGuiViewport* vp = ImGui::GetMainViewport();

    //// 창 위치를 (0,0)으로 설정
   
    ImGui::SetNextWindowViewport(vp->ID);

    m_ImGuiWindowFlags = ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoDecoration |
                         ImGuiWindowFlags_NoNav |
                         ImGuiWindowFlags_NoBackground |
                         ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_MenuBar;

    // 패딩과 테두리를 고려해 창 스타일 비활성화
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    return S_OK;
}

HRESULT CImgManager::Setting_Img_UI()
{
    if (FAILED(ADD_IMG_UserInterface(TEXT("Profiler"), CIMG_Profiler::Create())))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("Hierarchy"), CIMG_Hierarchy::Create())))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("Inspector"), CIMG_Inspector::Create())))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("LandScape"), CIMG_LandScape::Create())))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("ViewPort"), CIMG_Viewport::Create())))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("Content"), CIMG_Content::Create())))
        return E_FAIL;

    return S_OK;
}

void CImgManager::CreateDockSpace()
{
    ImGuiID dockspace_id = ImGui::GetID("MainDock");
    ImGuiDockNodeFlags dock_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dock_flags);
}

void CImgManager::DarwMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {


            ImGui::EndMenuBar();
        }

        ImGui::EndMenuBar();
    }
}

void CImgManager::Free()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    ClearAllSelectObjects();
   
    for (auto& pair : m_ImgUIMap)
        Safe_Release(pair.second);

    m_ImgUIMap.clear();
}
#endif //  _DEBUG