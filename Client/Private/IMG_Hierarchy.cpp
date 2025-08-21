#include "IMG_Hierarchy.h"

#include "GameInstance.h"
#include "ImgManager.h"

#include "StringHelper.h"
#include "Level.h"

CIMG_Hierarchy::CIMG_Hierarchy() :
	CImgUIBase()
{
}

HRESULT CIMG_Hierarchy::Prototype_Initialize()
{
    m_pImgManager = CImgManager::GetInstance();

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

        for (auto& Obj : m_pParentNodes)
        {
            Draw_Hierarchy(Obj);
            Safe_Release(Obj);
        }
        m_pParentNodes.clear();

        if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_DELETE))
        {
            auto SelectObjects = m_pImgManager->GetSelectObjects();
            for (auto& iter : *SelectObjects)
                iter->SetDead(true);

            m_pImgManager->ClearAllSelectObjects();
        }

        if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_END))
        {
            auto SelectObjects = m_pImgManager->GetSelectObjects();

            // 선택된 모든 오브젝트기준으로 아래로 Ray를 쏴서
            // 검출된 오브젝트를 확인
            for (auto& iter : *SelectObjects)
            {
                _float3 vRayPos = iter->GetTransform()->GetPosition();
                _vector RayStart = XMLoadFloat3(&vRayPos);
                _vector RayDir = -iter->GetTransform()->GetUpVector();

                _uInt iLevelID = m_pGameInstance->GetCurrentLevel()->GetLevelID();
                auto GroundObjects = m_pGameInstance->GetAllObejctToLayer(iLevelID, TEXT("Layer_GamePlay_Terrian"));

                for (auto Ground : *GroundObjects)
                {
                    auto VIBuffer = static_cast<CVIBuffer*>(Ground->Find_Component(TEXT("VIBuffer_Com")));

                    _float3 vPos{};
                    if (VIBuffer->IsPicking(RayStart, RayDir, Ground->GetTransform(), &vPos))
                    {
                        _float y = vPos.y;
                        XMStoreFloat3(&vPos, RayStart);
                        vPos.y = y;

                        iter->SetLocation(vPos);
                    }
                }
            }
        }

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

HRESULT CIMG_Hierarchy::Draw_Hierarchy(CGameObject* pObject)
{
    //노드 표현했으면 부모 래퍼런스 감소 그다음 다그리면 클리어
    m_TreeNodeFlag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    CStringHelper::ConvertWideToUTF(pObject->GetObjectTag().c_str(), m_szName);
    sprintf_s(m_szName, "%s##%d", m_szName, 0);

    /* if (m_SelectedObject == Objects)
         flags |= ImGuiTreeNodeFlags_Selected;*/

         //Node Ex는 오픈했는지 안했는지를 반환해 주는 형태의 함수

    if (nullptr == pObject->GetChildObject())
        return E_FAIL;

    auto SelectObjects = m_pImgManager->GetSelectObjects();
    auto iter = find(SelectObjects->begin(), SelectObjects->end(), pObject);
    if (iter != SelectObjects->end())
        m_TreeNodeFlag |= ImGuiTreeNodeFlags_Selected;

    bool opened = ImGui::TreeNodeEx(m_szName, m_TreeNodeFlag);
    WCHAR wstr[1024] = {};

    // 선택 감지 (펼치기와 별개로) 
    if (ImGui::IsItemClicked())
    {
        if (m_TreeNodeFlag & ImGuiTreeNodeFlags_Selected)
            m_pImgManager->Remove_SelectObject(pObject);
        else
            m_pImgManager->ADD_SelectObject(pObject);
    }
    if (opened)
    {
        for (auto& iter : *pObject->GetChildObject())
        {
            Draw_Hierarchy(iter);
        }

        ImGui::TreePop();
    }

    return S_OK;
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
