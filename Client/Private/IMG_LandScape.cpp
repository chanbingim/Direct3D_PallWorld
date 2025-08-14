#include "IMG_LandScape.h"

#include "GameInstance.h"
#include "Terrian.h"
#include "Level.h"

CIMG_LandScape::CIMG_LandScape() :
    CImgUIBase()
{
}

HRESULT CIMG_LandScape::Prototype_Initialize()
{
    for (int i = 0; i < 8; ++i)
    {
        sprintf_s(m_szQuadPreview[i], sizeof(m_szQuadPreview[i]), "%d x %d", (int)pow(2, i + 1), (int)pow(2, i + 1));
    }

    return S_OK;
}

HRESULT CIMG_LandScape::Initialize(void* pArg)
{

    return S_OK;
}

void CIMG_LandScape::Update(_float fDeletaTime)
{
    if (ImGui::Begin("LandScape"))
    {
        ImGui::SetNextItemWidth(200);
        if (ImGui::BeginCombo("Quad", m_SelectQuad))
        {
            for (int i = 0; i < 8; ++i)
            {
                if (ImGui::Selectable(m_szQuadPreview[i], false))
                {
                    m_SelectQuad = m_szQuadPreview[i];
                    m_QuadPow = i + 1;
                }
            }
            ImGui::EndCombo();
        }

        DrawTileCount();

        if (ImGui::Button("Generate"))
            GenerateTerrian();

        ImGui::End();
    }
}

void CIMG_LandScape::DrawTileCount()
{
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("##TileCountX", &m_tileCount[0], 0);
    ImGui::SameLine();
    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("##TileCountY", &m_tileCount[1], 0);
    ImGui::SameLine();
    ImGui::Text("Tile Count");
}

void CIMG_LandScape::GenerateTerrian()
{
    //현재 레벨에다가 추가
    auto CurLevel = m_pGameInstance->GetCurrentLevel()->GetLevelID();

    //타일 n x n 의 크기를 지정
    _uInt GridX = pow(2, m_QuadPow);

    // 개수 및 위치를 넘겨서 추가하기
    for (int i = 0; i < m_tileCount[0]; ++i)
    {
        for (int j = 0; j < m_tileCount[1]; ++j)
        {
            //구조체 세팅해서 넘긴다음 Clone
            CTerrian::TERRIAN_DESC Desc;
            ZeroMemory(&Desc, sizeof(CTerrian::TERRIAN_DESC));

            Desc.vPosition = { (_float)GridX  * j, 0.f, (_float)GridX * i };
            Desc.vScale = { 1.f, 1.f, 1.f };
            Desc.iGridCnt = GridX;
            
            if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(CurLevel, TEXT("Prototype_GameObject_Terrian"), CurLevel,
                TEXT("Layer_GamePlay_BackGround"), &Desc)))
                return;

        }
    }
}

CIMG_LandScape* CIMG_LandScape::Create()
{
    CIMG_LandScape* pLandScape = new CIMG_LandScape();
    if (FAILED(pLandScape->Prototype_Initialize()))
    {
        Safe_Release(pLandScape);
        MSG_BOX("CREATE FAIL : LAND SCAPE");
    }
    return pLandScape;
}

CImgUIBase* CIMG_LandScape::Clone(void* pArg)
{
    CIMG_LandScape* pLandScape = new CIMG_LandScape();
    if (FAILED(pLandScape->Initialize(pArg)))
    {
        Safe_Release(pLandScape);
        MSG_BOX("CLONE FAIL : LAND SCAPE");
    }
    return pLandScape;
}

void CIMG_LandScape::Free()
{
    __super::Free();
}
