#include "BaseCamera.h"

#include "GameInstance.h"

CBaseCamera::CBaseCamera(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CGameObject(pGraphic_Device, pDeviceContext)
{

}

CBaseCamera::CBaseCamera(const CBaseCamera& rhs) :
    CGameObject(rhs),
    m_FustomPointVec(rhs.m_FustomPointVec),
    m_fFar(rhs.m_fFar),
    m_fAspect(rhs.m_fAspect),
    m_fNear(rhs.m_fNear),
    m_fFov(rhs.m_fFov)
{
}

HRESULT CBaseCamera::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    // Near부분 평면
    m_FustomPointVec.push_back({ -1.f, 1.f, 0.f});
    m_FustomPointVec.push_back({ 1.f, 1.f,  0.f});
    m_FustomPointVec.push_back({ 1.f, -1.f, 0.f});
    m_FustomPointVec.push_back({ -1.f, -1.f, 0.f});

    // Far부분 평면
    m_FustomPointVec.push_back({ -1.f, 1.f, 1.f });
    m_FustomPointVec.push_back({ 1.f, 1.f,  1.f });
    m_FustomPointVec.push_back({ 1.f, -1.f, 1.f });
    m_FustomPointVec.push_back({ -1.f, -1.f, 1.f });

    return S_OK;
}

HRESULT CBaseCamera::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;
    
    CAMERA_DESC*  Desc = static_cast<CAMERA_DESC*>(pArg);
    if (Desc)
    {
        D3D11_VIEWPORT viewPortDesc = {};
        _uInt iViewprotNum = { 1 };
        m_pDeviceContext->RSGetViewports(&iViewprotNum, &viewPortDesc);

        m_fFov = Desc->fFov;
        m_fAspect = viewPortDesc.Width / viewPortDesc.Height;
        m_fFar = Desc->fFar;
        m_fNear = Desc->fNear;

        m_pTransformCom->SetPosition(Desc->vEye);
        m_pTransformCom->LookAt(XMLoadFloat3(&Desc->vAt));

        XMStoreFloat4x4(&m_ProjMat, XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNear, m_fFar));
        
        m_pGameInstance->SetMatrix(MAT_STATE::VIEW, m_pTransformCom->GetInvWorldMat());
        m_pGameInstance->SetMatrix(MAT_STATE::PROJECTION, m_ProjMat);
    }

    Compute_FustomPlane();

    return S_OK;
}

void CBaseCamera::Priority_Update(_float fDeletaTime)
{
    XMStoreFloat4x4(&m_ProjMat, XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNear, m_fFar));

    m_pGameInstance->SetMatrix(MAT_STATE::VIEW, m_pTransformCom->GetInvWorldMat());
    m_pGameInstance->SetMatrix(MAT_STATE::PROJECTION, m_ProjMat);
    Compute_FustomPlane();
}

void CBaseCamera::Update(_float fDeletaTime)
{
    
}

void CBaseCamera::Late_Update(_float fDeletaTime)
{
}

HRESULT CBaseCamera::Render()
{
    return S_OK;
}

void CBaseCamera::SetAspect(_float fWidth, _float fHeight)
{
    if (fWidth && fHeight)
        m_fAspect = fWidth / fHeight;
}

void CBaseCamera::CameraLookAt(_vector vLookAt)
{
    m_pTransformCom->LookAt(vLookAt);
}

void CBaseCamera::SetFov(_float fAngle)
{
    m_fFov = XMConvertToRadians(fAngle);
}

_bool CBaseCamera::IsInPoint(_float3& vPos, _float offset)
{
    for (auto& iter : m_FustomPlane)
    {
        float d = (iter.x * vPos.x + iter.y * vPos.y + iter.z * vPos.z) + iter.w;
        if (d > offset)
            return false;
    }

    return true;
}

void CBaseCamera::Compute_FustomPlane()
{
    for (size_t i = 0; i < m_FustomPointVec.size(); ++i)
    {
        _float4 vWorldPoint = {};

        auto Mat = XMLoadFloat4x4(&m_pGameInstance->GetInvMatrix(MAT_STATE::PROJECTION));
        auto vWVPoint = XMVector3TransformCoord(XMLoadFloat3(&m_FustomPointVec[i]), Mat);
        m_FustomWorldPointVec.push_back(XMVector3TransformCoord(vWVPoint, XMLoadFloat4x4(&m_pGameInstance->GetInvMatrix(MAT_STATE::VIEW))));
    }

    if (6 <= m_FustomWorldPointVec.size())
    {
        XMStoreFloat4(&m_FustomPlane[0], XMPlaneFromPoints(m_FustomWorldPointVec[0], m_FustomWorldPointVec[1], m_FustomWorldPointVec[2]));
        XMStoreFloat4(&m_FustomPlane[1], XMPlaneFromPoints(m_FustomWorldPointVec[4], m_FustomWorldPointVec[0], m_FustomWorldPointVec[3]));
        XMStoreFloat4(&m_FustomPlane[2], XMPlaneFromPoints(m_FustomWorldPointVec[4], m_FustomWorldPointVec[5], m_FustomWorldPointVec[1]));

        XMStoreFloat4(&m_FustomPlane[3], XMPlaneFromPoints(m_FustomWorldPointVec[4], m_FustomWorldPointVec[5], m_FustomWorldPointVec[6]));
        XMStoreFloat4(&m_FustomPlane[4], XMPlaneFromPoints(m_FustomWorldPointVec[5], m_FustomWorldPointVec[6], m_FustomWorldPointVec[2]));
        XMStoreFloat4(&m_FustomPlane[5], XMPlaneFromPoints(m_FustomWorldPointVec[7], m_FustomWorldPointVec[6], m_FustomWorldPointVec[2]));
        m_FustomWorldPointVec.clear();
    }
}

CGameObject* CBaseCamera::Clone(void* pArg)
{
    return nullptr;
}

void CBaseCamera::Free()
{
    __super::Free();
}
