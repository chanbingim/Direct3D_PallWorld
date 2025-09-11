#ifdef  _DEBUG

#include "ImgManager.h"

#include "GameInstance.h"
#include "StringHelper.h"
#include "Level.h"
#include "GameObject.h"

#include "Terrain.h"
#include "Enviormnent.h"
#include "Camera.h"

#pragma region IMG_UI_HEADER
#include "IMG_Profiler.h"
#include "IMG_Hierarchy.h"
#include "IMG_Inspector.h"
#include "IMG_LandScape.h"
#include "IMG_Viewport.h"
#include "IMG_Content.h"
#include "IMG_Create.h"
#include "IMG_ModelConvert.h"
#pragma endregion


IMPLEMENT_SINGLETON(CImgManager);

HRESULT CImgManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    if (FAILED(Default_Setting(pDevice, pContext)))
        return E_FAIL;

    if (FAILED(Setting_Img_UI(pDevice, pContext)))
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
        {
            if (VISIBILITY::HIDDEN == pair.second->GetVisibility())
                continue;

            pair.second->Update(fDeletaTime);
        }
        ImGui::End();
    }
}

void CImgManager::Render_Begin()
{
    for (auto& pair : m_ImgUIMap)
    {
        if (VISIBILITY::HIDDEN == pair.second->GetVisibility())
            continue;

        pair.second->Render();
    }
}

void CImgManager::Render_End()
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

HRESULT CImgManager::Setting_Img_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    if (FAILED(ADD_IMG_UserInterface(TEXT("Profiler"), CIMG_Profiler::Create(pDevice, pContext))))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("Hierarchy"), CIMG_Hierarchy::Create(pDevice, pContext))))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("Inspector"), CIMG_Inspector::Create(pDevice, pContext))))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("LandScape"), CIMG_LandScape::Create(pDevice, pContext))))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("ViewPort"), CIMG_Viewport::Create(pDevice, pContext))))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("Content"), CIMG_Content::Create(pDevice, pContext))))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("IMG_Create"), CIMG_Create::Create(pDevice, pContext))))
        return E_FAIL;

    if (FAILED(ADD_IMG_UserInterface(TEXT("IMG_ModelConvertView"), CIMG_ModelConvert::Create(pDevice, pContext))))
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
            if (ImGui::MenuItem("Save"))
            {
                SaveLevel();
            }

            if (ImGui::MenuItem("Load"))
            {
                LoadLevel();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("ModelView"))
            {
                auto IMG_UIObject = Find_ImgUserInterface(L"IMG_ModelConvertView");
                if (IMG_UIObject)
                    IMG_UIObject->SetVisibility(VISIBILITY::VISIBLE);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

HRESULT CImgManager::SaveLevel()
{
    char FilePath[MAX_PATH] = "";
    char LayerName[MAX_PATH] = "";

    auto pGameInstance = CGameInstance::GetInstance();
    _uInt iCurLevel = pGameInstance->GetCurrentLevel()->GetLevelID();
  
    auto CurLevelLayers = pGameInstance->GetCurLevelLayer();
    for (auto& LayerPair : *CurLevelLayers)
    {
        strcpy_s(FilePath, "../Bin/Save/Map/");
        list<SAVE_LEVEL_DESC> SaveList;
        auto CurObjectList = pGameInstance->GetAllObejctToLayer(iCurLevel, LayerPair.first);
        CStringHelper::ConvertWideToUTF(LayerPair.first.c_str(), LayerName);

        SAVE_OBJECT_TYPE type;
        if (_wstring::npos != LayerPair.first.find(L"Camera"))
            type = SAVE_OBJECT_TYPE::CAMERA;
        else if (_wstring::npos != LayerPair.first.find(L"Terrian"))
            type = SAVE_OBJECT_TYPE::TERRAIN;
        else if (_wstring::npos != LayerPair.first.find(L"Enviorment"))
            type = SAVE_OBJECT_TYPE::ENVIORNMENT;
        else
            type = SAVE_OBJECT_TYPE::OBEJCT;

        sprintf_s(FilePath, "%s%s.txt", FilePath, LayerName);
        for (auto& Object : *CurObjectList)
        {
            SAVE_LEVEL_DESC PaserData;
            ZeroMemory(&PaserData, sizeof(SAVE_LEVEL_DESC));
            PaserData.eType = ENUM_CLASS(type);
            PaserData.iSaveLevelID = iCurLevel;
            strcpy_s(PaserData.LayerName, LayerName);
            Object->ExportData(&PaserData);
            SaveList.push_back(PaserData);
        }

        SaveFile(FilePath, SaveList);
    }
   
    return S_OK;
}

HRESULT CImgManager::LoadLevel()
{
    for(_Int i= 0; i < 4; ++i)
        LoadFile(i);

    return S_OK;
}

void CImgManager::SaveFile(const char* FilePath, list<SAVE_LEVEL_DESC>& SaveData)
{
    //파일 입출력 열어서 저장
    ios_base::openmode flag;
    flag = ios::out | ios::trunc;
    ofstream file(FilePath, flag);

    if (file.is_open())
    {
        _uInt iSaveObjectCnt = (_uInt)SaveData.size();
        file<< iSaveObjectCnt << endl;
        for (SAVE_LEVEL_DESC& iter : SaveData)
        {
            file << iter.eType << " " << iter.iPrototypeLevelID << " " << iter.iSaveLevelID << " ";
            file << iter.PrototypeName << " " << iter.LayerName << " " << iter.PrototypeIndex << " ";
            file << iter.vScale.x << " " << iter.vScale.y << " " << iter.vScale.z << " ";
            file << iter.vRotation.x << " " << iter.vRotation.y << " " << iter.vRotation.z << " ";
            file << iter.vPosition.x << " " << iter.vPosition.y << " " << iter.vPosition.z << " ";

            switch (SAVE_OBJECT_TYPE(iter.eType))
            {
            case SAVE_OBJECT_TYPE::CAMERA:
                file << iter.ObjectDesc.CameraDesc.CameraType << " ";
                file << iter.ObjectDesc.CameraDesc.fNear << " ";
                file << iter.ObjectDesc.CameraDesc.fFar << " ";
                file << iter.ObjectDesc.CameraDesc.fFov << " ";
                break;
            case SAVE_OBJECT_TYPE::TERRAIN:
                file << iter.ObjectDesc.TerrianDesc.TerrainType << " ";
                file << iter.ObjectDesc.TerrianDesc.TileCnt.x << " " << iter.ObjectDesc.TerrianDesc.TileCnt.y << " ";
                file << iter.ObjectDesc.TerrianDesc.HeightMapCom << " ";
                file << iter.ObjectDesc.TerrianDesc.NaviMeshPath;
                break;
            case SAVE_OBJECT_TYPE::OBEJCT:
                break;
            case SAVE_OBJECT_TYPE::ENVIORNMENT:
                break;
            }
            file << endl;
        }
    }

    file.close();
}

void CImgManager::LoadFile(_uInt iType)
{
    //파일 입출력 열어서 저장
    ios_base::openmode flag;
    flag = ios::in;

    list<SAVE_LEVEL_DESC> LoadList;
    auto pGameInstance = CGameInstance::GetInstance();
    char FilePath[MAX_PATH] = "";
    SAVE_OBJECT_TYPE ObjectType = SAVE_OBJECT_TYPE(iType);
    switch (ObjectType)
    {
    case SAVE_OBJECT_TYPE::CAMERA :
        strcpy_s(FilePath, "../Bin/Save/Map/Layer_GamePlay_Camera.txt");
        break;
    case SAVE_OBJECT_TYPE::TERRAIN:
        strcpy_s(FilePath, "../Bin/Save/Map/Layer_GamePlay_Terrian.txt");
        break;
    case SAVE_OBJECT_TYPE::ENVIORNMENT:
        strcpy_s(FilePath, "../Bin/Save/Map/Layer_GamePlay_Enviorment.txt");
        break;
    case SAVE_OBJECT_TYPE::OBEJCT:
        strcpy_s(FilePath, "../Bin/Save/Map/Layer_GamePlay_Player.txt");
        break;
    }

    ifstream file(FilePath, flag);
    if (file.is_open())
    {
        _uInt iLoadObjectCnt = {};
        file.read(reinterpret_cast<_char*>(&iLoadObjectCnt), sizeof(_uInt));
        for (_uInt i = 0; i < iLoadObjectCnt; ++i)
        {
            SAVE_LEVEL_DESC LoadData;
            file.read(reinterpret_cast<_char*>(&LoadData), sizeof(SAVE_LEVEL_DESC));
            LoadList.push_back(LoadData);
        }
    }
    file.close();

    WCHAR szPrototypeName[MAX_PATH] = {};
    WCHAR szLayerName[MAX_PATH] = {};
    _uInt ObejctCnt = {};
    for (auto& iter : LoadList)
    {
        CGameObject::GAMEOBJECT_DESC* Desc = nullptr;
#pragma region Bind
        switch (ObjectType)
        {
        case SAVE_OBJECT_TYPE::CAMERA:
        {
            Desc = new CCamera::CAMERA_DESC;
            ZeroMemory(Desc, sizeof(CCamera::CAMERA_DESC));

            CCamera::CAMERA_DESC* CameraDesc = static_cast<CCamera::CAMERA_DESC*>(Desc);
            CameraDesc->vEye = iter.vPosition;
            CameraDesc->vAt = {0, 0, 0};
            CameraDesc->fFar = iter.ObjectDesc.CameraDesc.fFar;
            CameraDesc->fNear = iter.ObjectDesc.CameraDesc.fNear;
            CameraDesc->fFov = iter.ObjectDesc.CameraDesc.fFov;

            wsprintf(Desc->ObjectTag, TEXT("Camera %d"), ObejctCnt);
        }
        break;
        case SAVE_OBJECT_TYPE::TERRAIN:
        {
            Desc = new CTerrain::TERRIAN_DESC;
            ZeroMemory(Desc, sizeof(CTerrain::TERRIAN_DESC));

            CTerrain::TERRIAN_DESC* TerrianDesc = static_cast<CTerrain::TERRIAN_DESC*>(Desc);
            TerrianDesc->TerrianType = iter.ObjectDesc.TerrianDesc.TerrainType;
            TerrianDesc->iGridCnt = (_uInt)iter.ObjectDesc.TerrianDesc.TileCnt.x;
            //TerrianDesc-> = iter.ObjectDesc.TerrianDesc.TileCnt.x;

            wsprintf(Desc->ObjectTag, TEXT("Terrian %d"), ObejctCnt);
        }
        break;
        case SAVE_OBJECT_TYPE::ENVIORNMENT:
        {
            Desc = new CEnviormnent::ENVIORMNENT_DESC;
            ZeroMemory(Desc, sizeof(CEnviormnent::ENVIORMNENT_DESC));

            CEnviormnent::ENVIORMNENT_DESC* EnvDesc = static_cast<CEnviormnent::ENVIORMNENT_DESC*>(Desc);
            EnvDesc->iModelIndex = iter.PrototypeIndex;
            wsprintf(Desc->ObjectTag, TEXT("Enviroment %d"), ObejctCnt);
        }
        break;
        case SAVE_OBJECT_TYPE::OBEJCT:
        {
            Desc = new CGameObject::GAMEOBJECT_DESC;
            ZeroMemory(Desc, sizeof(CGameObject::GAMEOBJECT_DESC));

            wsprintf(Desc->ObjectTag, TEXT("GameObject %d"), ObejctCnt);
        }
        break;
        }
#pragma endregion
        if (nullptr == Desc)
            return;

        Desc->vScale = iter.vScale;
        Desc->vRotation = iter.vRotation;
        Desc->vPosition = iter.vPosition;

        CStringHelper::ConvertUTFToWide(iter.PrototypeName, szPrototypeName);
        CStringHelper::ConvertUTFToWide(iter.LayerName, szLayerName);
       
       
        pGameInstance->Add_GameObject_ToLayer(iter.iPrototypeLevelID, szPrototypeName, iter.iSaveLevelID, szLayerName, Desc);
        ObejctCnt++;

        Safe_Delete(Desc);
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