#include "IMG_Viewport.h"

#include "GameInstance.h"
#include "ImgManager.h"

CIMG_Viewport::CIMG_Viewport()
{
}

HRESULT CIMG_Viewport::Prototype_Initialize()
{
    m_szModeName[0] = "Select";
    m_szModeName[1] = "LandScape";

    return S_OK;
}

HRESULT CIMG_Viewport::Initialize(void* pArg)
{
    return S_OK;
}

void CIMG_Viewport::Update(_float fDeletaTime)
{
    ImVec2 DrawSize = ImGui::GetWindowContentRegionMax();
    ImGui::SetNextWindowSize(DrawSize);
    if (ImGui::Begin("ViewPort", 0, ImGuiWindowFlags_AlwaysAutoResize))
    {
        DrawViewPortMenuBar();
        DrawGameView();
        IsViewportClicked();
        ImGui::End();
    }
}

void CIMG_Viewport::Change_EditMode(EDIT_MODE eMode)
{
    m_eEditMode = eMode;
}

const EDIT_MODE& CIMG_Viewport::GetEditMode()
{
    return m_eEditMode;
}

void CIMG_Viewport::DrawViewPortMenuBar()
{
    ImGui::SetNextWindowSize(ImVec2(200, 50));
    if (ImGui::BeginCombo("Select", m_szSelect))
    {
        for (_uInt i = 0; i < ENUM_CLASS(EDIT_MODE::END); ++i)
        {
            if (ImGui::Selectable(m_szModeName[i], false))
            {
                strcpy_s(m_szSelect, strlen(m_szModeName[i]) + 1, m_szModeName[i]);

            }
        }
        ImGui::EndCombo();
    }
}

void CIMG_Viewport::DrawGameView()
{
    //내가 현재 그릴수있는 영역을 반환해준다.
    m_ViewFrameSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

    //m_ViewFrameSize = { g_iWinSizeX, g_iWinSizeY };
    //내가 그릴 UI의 좌측 상단의 좌표 즉 Min의 좌표를 반환해줌
    ImVec2 CursorPos = ImGui::GetCursorScreenPos();
    m_CneterPos = { CursorPos.x, CursorPos.y };

    m_pGameInstance->SetEditor_Frame(m_ViewFrameSize);
    //랜더링된 화면을 UI표시하기위해서 영역, 이미지를 지정해준다.
    ImGui::Image(m_pGameInstance->GetPostBuffer(0), ImGui::GetContentRegionAvail());
}

void CIMG_Viewport::IsViewportClicked()
{
    m_pGameInstance->Change_Mode(GAMEMODE::GAME);
    //UI위에 마우스를 올렸을때만 위치갱신이 되게 하자
    if (ImGui::IsItemHovered())
    {
        //마우스 위치 갱신
        ImVec2 MousePos = ImGui::GetMousePos();
        ImVec2 ViewportPos = { MousePos.x - m_CneterPos.x,  MousePos.y - m_CneterPos.y };

        m_pGameInstance->SetEditor_MousePos({ ViewportPos.x, ViewportPos.y, 0.f});
    }
}

CIMG_Viewport* CIMG_Viewport::Create()
{
    CIMG_Viewport* pViewport = new CIMG_Viewport();
    if (FAILED(pViewport->Prototype_Initialize()))
    {
        Safe_Release(pViewport);
        MSG_BOX("CREATE FAIL : VIEW PORT");
    }
    return pViewport;
}

void CIMG_Viewport::Free()
{
    __super::Free();
}
