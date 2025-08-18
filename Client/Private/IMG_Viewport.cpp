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
    ImVec2 DrawSize = ImGui::GetContentRegionAvail();
    ImGui::Image(m_pGameInstance->GetPostBuffer(0), DrawSize);
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
