#include "IMG_Inspector.h"

#include "GameInstance.h"

#include "ImgManager.h"
#include "GameObject.h"

#pragma region Inspector UI
#include "IMG_Transform.h"
#pragma endregion

CIMG_Inspector::CIMG_Inspector()
{
}

HRESULT CIMG_Inspector::Prototype_Initialize()
{
    if (FAILED(Default_Setting()))
        return E_FAIL;

    m_pImgManager = CImgManager::GetInstance();

	return S_OK;
}

HRESULT CIMG_Inspector::Initialize(void* pArg)
{
	return S_OK;
}

void CIMG_Inspector::Update(_float fDeletaTime)
{
    Update_UIState();
    if (ImGui::Begin("Inspector"))
    {
        for (auto& pair : m_InspectorUIMap)
        {
            CComponent* pComponent = nullptr;
            if (m_pSelectObj)
            {
                pComponent = m_pSelectObj->Find_Component(pair.first);
                pair.second.second = true;
            }
            else
                pair.second.second = false;

            if (pair.second.second)
            {
               pair.second.first->Bind_Data(m_pSelectObj, pComponent);
               pair.second.first->Update(fDeletaTime);
            }
        }
        ImGui::End();
    }
}

HRESULT CIMG_Inspector::Default_Setting()
{
    if (FAILED(ADD_InspectorUI(TEXT("Transform_Com"), CIMG_Transform::Create())))
        return E_FAIL;

    return S_OK;
}

HRESULT CIMG_Inspector::ADD_InspectorUI(const WCHAR* szTag, CIMG_Component* pUserInterface)
{
    auto pair = m_InspectorUIMap.find(szTag);
    if (pair == m_InspectorUIMap.end())
        m_InspectorUIMap.emplace(szTag, make_pair(pUserInterface, true));
    else
    {
        MSG_BOX("Already Created : UI");
        return E_FAIL;
    }

    return S_OK;
}

void CIMG_Inspector::Update_UIState()
{
    auto SelectObjects = m_pImgManager->GetSelectObjects();
    if (1 == SelectObjects->size())
        m_pSelectObj = *(SelectObjects->begin());
    else
        m_pSelectObj = nullptr;
}

CIMG_Inspector* CIMG_Inspector::Create()
{
    CIMG_Inspector* pInspector = new CIMG_Inspector();
    if (FAILED(pInspector->Prototype_Initialize()))
    {
        Safe_Release(pInspector);
        MSG_BOX("CREATE FAIL : IMG INSPECTOR");
    }
    return pInspector;
}

CImgUIBase* CIMG_Inspector::Clone(void* pArg)
{
    CIMG_Inspector* pInspector = new CIMG_Inspector();
    if (FAILED(pInspector->Initialize(pArg)))
    {
        Safe_Release(pInspector);
        MSG_BOX("CLONE FAIL : IMG INSPECTOR");
    }
    return pInspector;
}

void CIMG_Inspector::Free()
{
    __super::Free();

    for (auto& pair : m_InspectorUIMap)
        Safe_Release(pair.second.first);

    m_InspectorUIMap.clear();
}
