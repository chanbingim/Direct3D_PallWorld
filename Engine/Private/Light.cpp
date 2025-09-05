#include "Light.h"

CLight::CLight(LIGHT_DESC LightInfo) :
    m_LightInfo(LightInfo)
{
}

CLight* CLight::Create(const LIGHT_DESC& Desc)
{
    return new CLight(Desc);
}

void CLight::Free()
{
    __super::Free();
}
