#include "Light.h"

#include "Shader.h"
#include "VIBuffer.h"

CLight::CLight(LIGHT_DESC LightInfo) :
    m_LightInfo(LightInfo)
{
}

HRESULT CLight::Render(CShader* pShader, CVIBuffer* pVIBuffer)
{
	_uInt			iPassIndex = { 0 };

	if (LIGHT::DIRECTION == m_LightInfo.eType)
	{
		if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightInfo.vDirection, sizeof(_float4))))
			return E_FAIL;
		iPassIndex = 1;
	}
	else
	{
		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightInfo.vPosition, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightInfo.fRange, sizeof(_float))))
			return E_FAIL;
		iPassIndex = 2;
	}

	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightInfo.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightInfo.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightInfo.vSpecular, sizeof(_float4))))
		return E_FAIL;

	pShader->Update_Shader(iPassIndex);
	pVIBuffer->Render_VIBuffer();
	return S_OK;
}

CLight* CLight::Create(const LIGHT_DESC& Desc)
{
    return new CLight(Desc);
}

void CLight::Free()
{
    __super::Free();
}
