#include "LightManager.h"
#include "Light.h"

CLightManager::CLightManager()
{
}

void CLightManager::ADDLight(CLight* pLight)
{
    auto iter = find(m_Lights.begin(), m_Lights.end(), pLight);
    if (iter == m_Lights.end())
        m_Lights.push_back(pLight);
   
}

const CLight* CLightManager::GetLight(_uInt iIndex)
{
    _uInt iLightIndex = {};
    for (auto& iter : m_Lights)
    {
        if (iLightIndex == iIndex)
            return iter;
        iLightIndex++;
    }

    return nullptr;
}

CLightManager* CLightManager::Create()
{
    return new CLightManager();
}

void CLightManager::Free()
{
    for (auto& iter : m_Lights)
        Safe_Release(iter);
}
