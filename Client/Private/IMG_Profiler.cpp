#include "IMG_Profiler.h"

#include "GameInstance.h"

CIMG_Profiler::CIMG_Profiler() :
    CImgUIBase()
{
    ZeroMemory(&m_fDurTime, sizeof(m_fDurTime));
}

HRESULT CIMG_Profiler::Prototype_Initialize()
{
    return S_OK;
}

HRESULT CIMG_Profiler::Initialize(void* pArg)
{
    return S_OK;
}

void CIMG_Profiler::Update(_float fDeletaTime)
{
    if (ImGui::Begin("Profiler"))
    {
        _float fPercent = {};

        m_fDurTime[0] = m_pGameInstance->Get_TimeDelta(TEXT("PriorityUpdate_Loop"), false);
        m_fDurTime[1] = m_pGameInstance->Get_TimeDelta(TEXT("Update_Loop"), false);
        m_fDurTime[2] = m_pGameInstance->Get_TimeDelta(TEXT("LateUpdate_Loop"), false);
        m_fDurTime[3] = m_pGameInstance->Get_TimeDelta(TEXT("Render_Loop"), false);

        for (int i = 0; i < 4; ++i)
        {
            ImGui::Text(GetProgressName(i));
            ImGui::SameLine();

            sprintf_s(m_szPercent,"%.2f", m_fDurTime[i]);
            ImGui::ProgressBar(m_fDurTime[i], ImVec2(200, 0), m_szPercent);
        }

        ImGui::End();
    }
}

const char* CIMG_Profiler::GetProgressName(_uInt iIndex)
{
    switch (iIndex)
    {
    case 0 :
        return "PriorityUpdate_Loop";
    case 1:
        return "Update_Loop";
    case 2:
        return "LateUpdate_Loop";
    case 3:
        return "Render_Loop";
    }

    return "";
}

CIMG_Profiler* CIMG_Profiler::Create()
{
    CIMG_Profiler* pProfiler = new CIMG_Profiler();
    if (FAILED(pProfiler->Prototype_Initialize()))
    {
        Safe_Release(pProfiler);
        MSG_BOX("CREATE FAIL : PROFILER");
    }
    return pProfiler;
}

CImgUIBase* CIMG_Profiler::Clone(void* pArg)
{
    CIMG_Profiler* pProfiler = new CIMG_Profiler(*this);
    if (FAILED(pProfiler->Initialize(pArg)))
    {
        Safe_Release(pProfiler);
        MSG_BOX("CLONE FAIL : PROFILER");
    }
    return pProfiler;
}

void CIMG_Profiler::Free()
{
    __super::Free();
}
