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
    if (m_SocketMatrix)
    {
        _matrix SocMatrix = XMLoadFloat4x4(m_SocketMatrix);
        SocMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f);
        SocMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f);
        SocMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f);

        XMStoreFloat4x4(&m_CombinedMatrix,
            XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()) * SocMatrix * XMLoadFloat4x4(&m_pParent->GetTransform()->GetWorldMat()));
    }
    else
    {
        _matrix RevolutionMatrix = XMMatrixRotationY(XMConvertToRadians(m_RevolutionAngle));
        _matrix ParentMatrix = XMLoadFloat4x4(&m_pParent->GetTransform()->GetWorldMat());
        _matrix CombinedMatrix = XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()) * RevolutionMatrix * XMMatrixTranslationFromVector(ParentMatrix.r[3]);
        XMStoreFloat4x4(&m_CombinedMatrix, CombinedMatrix);
    }

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

void CPlayerCamera::ADDRevolutionMatrix(_float Angle)
{
    m_RevolutionAngle += Angle;
    m_RevolutionAngle = fmodf(m_RevolutionAngle, 360.f);
    if (m_RevolutionAngle < 0.f)
        m_RevolutionAngle += 360.f;
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
