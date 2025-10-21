#include "IMG_LandScape.h"

#include "TerrainManager.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "Level.h"

#include "InstanceModel.h"
#include "WorkAbleObject.h"

const char* CIMG_LandScape::szBrushMode[ENUM_CLASS(BRUSH_MODE::END)] = {
       "SELECT_TERRIAN", "EDIT_ENVIORNMENT", "ADD_HEGIHT", "EDIT_NAVIMESH" };

CIMG_LandScape::CIMG_LandScape(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CImgUIBase(pDevice, pContext)
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
        ImGui::SetNextItemWidth(400);
        if (ImGui::Button("PreFab_View"))
            m_bIsPrefabBursh = !m_bIsPrefabBursh;

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

        ImGui::SameLine();

        if (ImGui::Button("Create HeightMap"))
        {
            //여기서 어떤 곳에다가 저장할지 결정한다음 저장
            CreateHeightMapToPng();
        }

        if (ImGui::Button("Refesh Hegiht"))
        {
            //여기서 어떤 곳에다가 저장할지 결정한다음 저장
            RefreshHegihtAllProbObject();
        }
    }
    ImGui::End();

    if (m_bIsPrefabBursh)
        DrawPrefabBrush();
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
    _uInt GridX = (_uInt)pow(2, m_QuadPow);

    // 개수 및 위치를 넘겨서 추가하기
    for (int i = 0; i < m_tileCount[0]; ++i)
    {
        float OffsetY = (i == 0 ? 0.f : 1.f);
        for (int j = 0; j < m_tileCount[1]; ++j)
        {
            float OffsetX = (j == 0 ? 0.f : 1.f);

            //구조체 세팅해서 넘긴다음 Clone
            CTerrain::TERRIAN_DESC Desc;
            ZeroMemory(&Desc, sizeof(CTerrain::TERRIAN_DESC));
            wsprintf(Desc.ObjectTag, TEXT("LandScape%d"), i * m_tileCount[1] + j);

            Desc.vPosition = { (_float)(GridX  * j - OffsetX), 0.f, (_float)(GridX * i - OffsetY) };
            Desc.vScale = { 1.f, 1.f, 1.f };
            Desc.iGridCnt = GridX;
            Desc.IsPicking = true;

            if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(CurLevel, TEXT("Prototype_GameObject_Terrian"), CurLevel,
                TEXT("Layer_GamePlay_Terrian"), &Desc)))
                return;
        }
    }
}

void CIMG_LandScape::DrawPrefabBrush()
{
    if (ImGui::Begin("PreFab_Brsuh"))
    {
        ImGui::SetNextItemWidth(150.f);
        ImGui::InputInt("BrushSize", &m_iBrushSize, 0);

        if (ImGui::BeginCombo("BRUSH_MODE", (char *)m_szBushModeName))
        {
            for (auto i = 0; i < ENUM_CLASS(BRUSH_MODE::END); ++i)
            {
                if (ImGui::Selectable(szBrushMode[i], false))
                {
                    m_eBrushMode = BRUSH_MODE(i);
                    strcpy_s(m_szBushModeName, szBrushMode[i]);
                }
            }

            ImGui::EndCombo();
        }

        ImGui::SetNextItemWidth(50.f);
        ImGui::InputInt("PreFab_Index", &m_iPrefabIndex, 0);

        ImGui::InputText("CreatePreFabPrototypeName", m_szPrafeName, MAX_PATH);
        ImGui::InputText("CreatePreFabLayerName", m_szLayerName, MAX_PATH);


    }
    ImGui::End();
}

void CIMG_LandScape::CreateHeightMapToPng()
{
    WCHAR FilePath[MAX_PATH] = {};
    //현재 레벨에다가 추가
    auto CurLevel = m_pGameInstance->GetCurrentLevel()->GetLevelID();
    const list<CGameObject*>* TerrianObject = m_pGameInstance->GetAllObejctToLayer(CurLevel, TEXT("Layer_GamePlay_Terrian"));

    for (auto Object : *TerrianObject)
    {
        wsprintf(FilePath, TEXT("../Bin/Save/HeightMap/%s.png"), Object->GetObjectTag().c_str());

        auto VITerrian = static_cast<CVIBuffer_Terrain *>(Object->Find_Component(TEXT("VIBuffer_Com")));
        VITerrian->ExportHeightMap(FilePath);
    }
}

void CIMG_LandScape::RefreshHegihtAllProbObject()
{
    auto CurLevel = m_pGameInstance->GetCurrentLevel()->GetLevelID();
    const list<CGameObject*>* EnvObject = m_pGameInstance->GetAllObejctToLayer(CurLevel, TEXT("Layer_GamePlay_Enviorment"));
    for (auto Object : *EnvObject)
        static_cast<CInstanceModel *>(Object)->RefreshComputeHeight();

    const list<CGameObject*>* WorkAbleObject = m_pGameInstance->GetAllObejctToLayer(CurLevel, TEXT("Layer_GamePlay_WorkAbleObject"));
    for (auto Object : *WorkAbleObject)
        static_cast<CWorkAbleObject*>(Object)->RefreshComputeHeight();
}

CIMG_LandScape* CIMG_LandScape::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CIMG_LandScape* pLandScape = new CIMG_LandScape(pDevice, pContext);
    if (FAILED(pLandScape->Prototype_Initialize()))
    {
        Safe_Release(pLandScape);
        MSG_BOX("CREATE FAIL : LAND SCAPE");
    }
    return pLandScape;
}

CImgUIBase* CIMG_LandScape::Clone(void* pArg)
{
    CIMG_LandScape* pLandScape = new CIMG_LandScape(*this);
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