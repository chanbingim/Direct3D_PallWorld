#include "IMG_Hierarchy.h"

#include "GameInstance.h"
#include "StringHelper.h"
#include "Level.h"

CIMG_Hierarchy::CIMG_Hierarchy() :
	CImgUIBase()
{
}

HRESULT CIMG_Hierarchy::Prototype_Initialize()
{
    
	return S_OK;
}

HRESULT CIMG_Hierarchy::Initialize(void* pArg)
{
	return S_OK;
}

void CIMG_Hierarchy::Update(_float fDeletaTime)
{
    if (ImGui::Begin("Hierarchy"))
    {
        Update_ParentNodes();
        Draw_Hierarchy();
        ImGui::End();
    }
}

void CIMG_Hierarchy::Update_ParentNodes()
{
    auto CurLevelLayers = m_pGameInstance->GetCurLevelLayer();
    _uInt CurLevelID = m_pGameInstance->GetCurrentLevel()->GetLevelID();
   
    for (auto& pair : *CurLevelLayers)
    {
        auto LayerObejcts = m_pGameInstance->GetAllObejctToLayer(CurLevelID, pair.first);
        
        //필요하다면 만들기 그룹화할때 말고는 필요없을거같음
        /*CStringHelper::ConvertWideToUTF(pair.first.c_str(), m_szName);
        ImGui::SeparatorText(m_szName);*/

        for (auto Object : *LayerObejcts)
        {
            if (nullptr == Object->GetParent())
            {
                //여기서 safe_Add_ref 를통해서 래퍼런스 카운트 관리
                 
                Safe_AddRef(Object);
                m_pParentNodes.push_back(Object);
            }
        }
    }
}

void CIMG_Hierarchy::Draw_Hierarchy()
{
    //노드 표현했으면 부모 래퍼런스 감소 그다음 다그리면 클리어
    _uInt iIndex = 0;
    for (auto Objects : m_pParentNodes)
    {
        m_TreeNodeFlag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

        CStringHelper::ConvertWideToUTF(Objects->GetObjectTag().c_str(), m_szName);
        sprintf_s(m_szName, "%s##%d", m_szName, iIndex);

        /* if (m_SelectedObject == Objects)
             flags |= ImGuiTreeNodeFlags_Selected;*/

             //Node Ex는 오픈했는지 안했는지를 반환해 주는 형태의 함수
        bool opened = ImGui::TreeNodeEx(m_szName, m_TreeNodeFlag);
        WCHAR wstr[1024] = {};

        // 선택 감지 (펼치기와 별개로) 
        if (ImGui::IsItemClicked())
        {
            //wsprintf(wstr, TEXT("Clicked : %s"), Objects->GetObjectTag().c_str());
            //SelectItem(Objects);
        }

        if (opened)
        {



            ImGui::TreePop();
        }

        Safe_Release(Objects);
        iIndex++;
    }
    m_pParentNodes.clear();
}

CIMG_Hierarchy* CIMG_Hierarchy::Create()
{
    CIMG_Hierarchy* pProfiler = new CIMG_Hierarchy();
    if (FAILED(pProfiler->Prototype_Initialize()))
    {
        Safe_Release(pProfiler);
        MSG_BOX("CREATE FAIL : Hierarchy");
    }
    return pProfiler;
}

CImgUIBase* CIMG_Hierarchy::Clone(void* pArg)
{
    CIMG_Hierarchy* pProfiler = new CIMG_Hierarchy(*this);
    if (FAILED(pProfiler->Initialize(pArg)))
    {
        Safe_Release(pProfiler);
        MSG_BOX("CLONE FAIL : Hierarchy");
    }
    return pProfiler;
}

void CIMG_Hierarchy::Free()
{
	__super::Free();

    for(auto& iter : m_pParentNodes)
        Safe_Release(iter);

    m_pParentNodes.clear();
}
