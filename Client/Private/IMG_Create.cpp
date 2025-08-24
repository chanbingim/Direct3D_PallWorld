#include "IMG_Create.h"

#include "GameInstance.h"
#include "StringHelper.h"
#include "Level.h"

CIMG_Create::CIMG_Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CImgUIBase(pDevice, pContext)
{
}

HRESULT CIMG_Create::Prototype_Initialize()
{
    m_SelectLevel = ENUM_CLASS(LEVEL::STATIC);
    strcpy_s(m_szSelectTag, strlen(m_SzSelectTagList[0]) + 1, m_SzSelectTagList[0]);
	return S_OK;
}

HRESULT CIMG_Create::Initialize(void* pArg)
{
	return S_OK;
}

void CIMG_Create::Update(_float fDeletaTime)
{
    if (ImGui::Begin("Create"))
    {
        DrawCreateCategory();
        ImGui::SameLine();
        DrawPrototypes();

        ImGui::End();
    }

}

void CIMG_Create::DrawCreateCategory()
{
    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo("Tag##LevelCategory", m_szSelectTag))
    {
        for (_uInt i = 0; i < 2; ++i)
        {
            if (ImGui::Selectable(m_SzSelectTagList[i], false))
            {
                strcpy_s(m_szSelectTag, strlen(m_SzSelectTagList[i]) + 1, m_SzSelectTagList[i]);
                
                if (i)
                    m_SelectLevel = m_pGameInstance->GetCurrentLevel()->GetLevelID();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    //List를 매프레임 재 생성한다.
    m_Prototypes.clear();

    //레벨 전환했을때 세팅하기위해 현재 레벨을 가져온다.
    _uInt LevelID = m_pGameInstance->GetCurrentLevel()->GetLevelID();
    if (0 < m_SelectLevel && m_SelectLevel != LevelID)
        m_SelectLevel = LevelID;

    m_pGameInstance->GetPrototypeList<CGameObject>(m_SelectLevel, &m_Prototypes);
}

void CIMG_Create::DrawPrototypes()
{
    if (ImGui::BeginCombo("Select##SelectPrototype", m_szSelectPrototype))
    {
        for (auto pair : m_Prototypes)
        {
            char ObjectName[MAX_PATH] = {};
            CStringHelper::ConvertWideToUTF(pair.first.c_str(), ObjectName);

            if (ImGui::Selectable(ObjectName, false))
            {
                strcpy_s(m_szSelectPrototype, strlen(ObjectName) + 1, ObjectName);
            }
        }
        ImGui::EndCombo();
    }

    //오브젝트 생성 숫자
    ImGui::PushItemWidth(200);
    ImGui::InputInt("Create Count", &m_iCreateCnt, 0);
    ImGui::PopItemWidth();

    //오브젝트를 생성할 레이어 이름
    ImGui::PushItemWidth(200);
    ImGui::InputText("Layer Name", m_szLayerName, MAX_PATH);
    ImGui::PopItemWidth();

    //오브젝트 Tag
    ImGui::PushItemWidth(200);
    ImGui::InputText("Tag Name", m_szObjectName, MAX_PATH);
    ImGui::PopItemWidth();

    //오브젝트 생성 버튼
    if (ImGui::Button("Create##ObjectGenerate", ImVec2(100, 20)))
    {
        //생성하라는 숫자를 통해서 생성하기
        for (_Int i = 0; i < m_iCreateCnt; ++i)
        {
            //현재 레벨 가져오기
            _uInt LevelID = m_pGameInstance->GetCurrentLevel()->GetLevelID();

            WCHAR   szWideProtoType[MAX_PATH] = {};
            WCHAR   szWideLayerName[MAX_PATH] = {};
            WCHAR   szWideObjectName[MAX_PATH] = {};

            CStringHelper::ConvertUTFToWide(m_szSelectPrototype, szWideProtoType);
            CStringHelper::ConvertUTFToWide(m_szLayerName, szWideLayerName);
            CStringHelper::ConvertUTFToWide(m_szObjectName, szWideObjectName);

            CGameObject::GAMEOBJECT_DESC Desc;
            ZeroMemory(&Desc, sizeof(CGameObject::GAMEOBJECT_DESC));
            wsprintf(Desc.ObjectTag, TEXT("%s%d"), szWideObjectName, i);
            Desc.vScale = { 1.f, 1.f, 1.f };

            m_pGameInstance->Add_GameObject_ToLayer(LevelID, szWideProtoType, LevelID, szWideLayerName, &Desc);
        }
    }
}

CIMG_Create* CIMG_Create::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CIMG_Create* pInspector = new CIMG_Create(pDevice, pContext);
    if (FAILED(pInspector->Prototype_Initialize()))
    {
        Safe_Release(pInspector);
        MSG_BOX("CREATE FAIL : IMG CREATE");
    }
    return pInspector;
}

CImgUIBase* CIMG_Create::Clone(void* pArg)
{
    CIMG_Create* pInspector = new CIMG_Create(*this);
    if (FAILED(pInspector->Initialize(pArg)))
    {
        Safe_Release(pInspector);
        MSG_BOX("CLONE FAIL : IMG CREATE");
    }
    return pInspector;
}

void CIMG_Create::Free()
{
	__super::Free();
}
