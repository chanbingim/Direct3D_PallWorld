#include "Shadow.h"

#include "Shader.h"
CShadow::CShadow() :
    CBase()
{
}

HRESULT CShadow::Ready_Shadow_Light(const SHADOW_LIGHT_DESC& Desc)
{
    XMStoreFloat4x4(&m_TransformationMatrices[ENUM_CLASS(MAT_STATE::VIEW)],
        XMMatrixLookAtLH(XMLoadFloat4(&Desc.vEye), XMLoadFloat4(&Desc.vAt), XMVectorSet(0.f, 1.f, 0.f, 0.f)));

    XMStoreFloat4x4(&m_TransformationMatrices[ENUM_CLASS(MAT_STATE::PROJECTION)],
        XMMatrixPerspectiveFovLH(Desc.fFov, Desc.fAspect, Desc.fNear, Desc.fFar));

    m_CameraDesc = Desc;
    return S_OK;
}

HRESULT CShadow::Bind_Shader_Resource(CShader* pShader, const _char* pConstantName, MAT_STATE eType)
{
    return pShader->Bind_Matrix(pConstantName, &m_TransformationMatrices[ENUM_CLASS(eType)]);
}

HRESULT CShadow::Bind_Shader_RawResource(CShader* pShader, const _char* pConstantName, _uInt iType)
{
    switch (iType)
    {
    case 0 :
        return pShader->Bind_RawValue(pConstantName, &m_CameraDesc.fFar, sizeof(_float));
    case 1:
        return pShader->Bind_RawValue(pConstantName, &m_CameraDesc.fNear, sizeof(_float));
    case 2:
        return pShader->Bind_RawValue(pConstantName, &m_CameraDesc.fFov, sizeof(_float));
    }
   
}

CShadow* CShadow::Create()
{
    return new CShadow();
}

void CShadow::Free()
{
    __super::Free();
}
