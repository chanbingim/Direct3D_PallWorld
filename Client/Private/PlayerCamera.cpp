#include "PlayerCamera.h"

#include "GameInstance.h"

CPlayerCamera::CPlayerCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBaseCamera(pDevice, pContext)
{
}

CPlayerCamera::CPlayerCamera(const CPlayerCamera& rhs) :
    CBaseCamera(rhs)
{
}

HRESULT CPlayerCamera::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerCamera::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    PLAYER_CAMERA_DESC* Desc = static_cast<PLAYER_CAMERA_DESC*>(pArg);
    m_SocketMatrix = Desc->pSocketMatrix;

    m_LerpFov = {60, 90};
    m_fStartLerpAngle = m_LerpFov.y;
    m_fFov = XMConvertToRadians(m_LerpFov.y);

    m_CameraModel = CAMERA_MODE::NONE_AIMMING;
    return S_OK;
}

void CPlayerCamera::Priority_Update(_float fDeletaTime)
{
   
}

void CPlayerCamera::Update(_float fDeletaTime)
{
   
}

void CPlayerCamera::Late_Update(_float fDeletaTime)
{
    if (m_bIsLerp)
    {
        _float fToAngle = {};
        m_fAccLerpTime += fDeletaTime;

        if (CAMERA_MODE::NONE_AIMMING == m_CameraModel)
            fToAngle = m_LerpFov.y;
        else  if (CAMERA_MODE::AIMING == m_CameraModel)
            fToAngle = m_LerpFov.x;
          
        m_fFov = XMConvertToRadians(Lerp<_float>(m_fStartLerpAngle, fToAngle, m_fAccLerpTime));
       
        if (m_fAccLerpTime >= 1.0f)
            m_bIsLerp = false;
    }

    _matrix ParentMatrix = XMLoadFloat4x4(&m_pParent->GetTransform()->GetWorldMat());
    _matrix CombinedMatrix{};

    if (CAMERA_MODE::NONE_AIMMING == m_CameraModel)
    {
        _vector vScale{}, vRotation{}, vPos{};
        _matrix WorldMat = XMLoadFloat4x4(&m_pTransformCom->GetWorldMat());
        XMMatrixDecompose(&vScale, &vRotation, &vPos, WorldMat);

        CombinedMatrix =  XMMatrixTranslationFromVector(vPos) * XMMatrixRotationQuaternion(vRotation) * XMMatrixTranslationFromVector(ParentMatrix.r[3]);
    }
    else
    {
        ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
        ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
        ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

        CombinedMatrix = XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()) * ParentMatrix;
    }
    XMStoreFloat4x4(&m_CombinedMatrix, CombinedMatrix);

    XMStoreFloat4x4(&m_InvCombinedMatrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_CombinedMatrix)));
    XMStoreFloat4x4(&m_ProjMat, XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNear, m_fFar));

    m_pGameInstance->SetMatrix(MAT_STATE::VIEW, m_InvCombinedMatrix);
    m_pGameInstance->SetMatrix(MAT_STATE::PROJECTION, m_ProjMat);
    Compute_FustomPlane();
}

HRESULT CPlayerCamera::Render()
{
    return E_NOTIMPL;
}

void CPlayerCamera::AttachCamera(_vector BonesTransformation)
{
    _float3 vPos = {};
    XMStoreFloat3(&vPos, BonesTransformation);
    SetLocation(vPos);
}

void CPlayerCamera::ADDRevolutionRotation(_float Angle, _float DeletaTime)
{
    m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(Angle), DeletaTime);
}

void CPlayerCamera::ADDPitchRotation(_float Angle, _float DeletaTime)
{
    _float rad = XMConvertToRadians(Angle) * DeletaTime;
    m_AccYawAngle += rad;

    m_AccYawAngle = Clamp<_float>(m_AccYawAngle, -XM_PIDIV4, XM_PIDIV4);
    if (-XM_PIDIV4 >= m_AccYawAngle)
    {
        m_AccYawAngle = -XM_PIDIV4;
        Angle = 0.f;
    }
    else if (XM_PIDIV4 <= m_AccYawAngle)
    {
        m_AccYawAngle = XM_PIDIV4;
        Angle = 0.f;
    }
    m_pTransformCom->Turn(m_pTransformCom->GetRightVector(), XMConvertToRadians(Angle), DeletaTime);
}

void CPlayerCamera::SetChangeCameraMode(CAMERA_MODE eMode)
{
    m_CameraModel = eMode;

    if (CAMERA_MODE::AIMING == m_CameraModel)
    {
        m_AccYawAngle = 0.f;
        SetRotation({ 0.f, 0.f, 0.f });
    }

    m_bIsLerp = true;
    m_fAccLerpTime = 0.f;
    m_fStartLerpAngle = XMConvertToDegrees(m_fFov);
}

CPlayerCamera* CPlayerCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerCamera* pCamera = new CPlayerCamera(pDevice, pContext);
    if (FAILED(pCamera->Initalize_Prototype()))
    {
        Safe_Release(pCamera);
        MSG_BOX("CREATE FAIL : PLAYER CAMERA");
    }

    return pCamera;
}

CGameObject* CPlayerCamera::Clone(void* pArg)
{
    CPlayerCamera* pCamera = new CPlayerCamera(*this);
    if (FAILED(pCamera->Initialize(pArg)))
    {
        Safe_Release(pCamera);
        MSG_BOX("CLONE FAIL : PLAYER CAMERA");
    }

    return pCamera;
}

void CPlayerCamera::Free()
{
    __super::Free();
}
