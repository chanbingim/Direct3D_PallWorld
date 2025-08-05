#include "Transform.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
    auto Matrix = XMMatrixIdentity();
    XMStoreFloat4x4(&m_WorldMat, Matrix);
    XMStoreFloat4x4(&m_InvWorldMat, Matrix);
    XMStoreFloat4x4(&m_ParentWorldMat, Matrix);
}

CTransform::CTransform(const CTransform& rhs) :
    CComponent(rhs),
    m_WorldMat(rhs.m_WorldMat),
    m_ParentWorldMat(rhs.m_ParentWorldMat),
    m_InvWorldMat(rhs.m_WorldMat)
{
}

_vector CTransform::GetRightVector()
{
    return XMLoadFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[0]));
}

_vector CTransform::GetUpVector()
{
    return XMLoadFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[1]));
}

_vector CTransform::GetLookVector()
{
    return XMLoadFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[2]));
}

_float3 CTransform::GetPosition()
{
    return *reinterpret_cast<_float3*>(&m_WorldMat.m[3]);
}

_float3 CTransform::GetScale()
{
    return _float3( 
        XMVectorGetX(XMVector3Length(GetRightVector())),
        XMVectorGetX(XMVector3Length(GetUpVector())),
        XMVectorGetX(XMVector3Length(GetLookVector())));
}

_float3 CTransform::GetRotation()
{
    return m_vRotation;
}

void CTransform::RotationAxis(_float3 vAxis, _float fAngle)
{
    _float3	vScale = GetScale();

    _matrix IdentiyMat = XMMatrixIdentity();
    _vector SacleVec = XMLoadFloat3(&vScale);

    auto AxisMat = XMMatrixRotationAxis(XMLoadFloat3(&vAxis), fAngle);
    for (_uInt i = 0; i < EnumToInt(WORLDSTATE::POSITION); ++i)
    {
        auto vNewRow = IdentiyMat.r[i] * SacleVec.m128_f32[i];
        XMStoreFloat4(reinterpret_cast<_float4 *>(&m_WorldMat.m[i]), XMVector3TransformNormal(vNewRow, AxisMat));
    }
}

void CTransform::SetPosition(_float3 vPosition)
{
    XMStoreFloat3(reinterpret_cast<_float3*>(&m_WorldMat.m[3]), XMLoadFloat3(&vPosition));
}

void CTransform::SetScale(_float3 vScale)
{
    _matrix WorldMat = XMLoadFloat4x4(&m_WorldMat);
    _vector Scale = XMLoadFloat3(&vScale);

    for (_uInt i = 0; i < EnumToInt(WORLDSTATE::POSITION); ++i)
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[i]), XMVector3Normalize(WorldMat.r[i]) * Scale.m128_f32[i]);
}

void CTransform::SetRotation(_float3 vRotation)
{
    _float3		vScale = GetScale();

    m_vRotation = vRotation;
    _matrix IdentiyMat = XMMatrixIdentity();
    _vector SacleVec = XMLoadFloat3(&vScale);

    _vector vQuternion = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&vRotation));
    _matrix QuternionMat = XMMatrixRotationQuaternion(vQuternion);

    for (_uInt i = 0; i < EnumToInt(WORLDSTATE::POSITION); ++i)
    {
        auto vNewRow = IdentiyMat.r[i] * SacleVec.m128_f32[i];
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[i]), XMVector3TransformNormal(vNewRow, QuternionMat));
    }
}

_float4x4& CTransform::GetWorldMat()
{
    if (m_bHasParent)
        XMStoreFloat4x4(&m_WorldMat, XMLoadFloat4x4(&m_WorldMat) * XMLoadFloat4x4(&m_ParentWorldMat));
   
    return m_WorldMat;
}

_float4x4& CTransform::GetInvWorldMat()
{
    XMStoreFloat4x4(&m_InvWorldMat, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMat)));
    return m_InvWorldMat;
}

void CTransform::SetParentMatrix(const _float4x4& matrix)
{
    m_bHasParent = true;
    auto ParentMat = XMLoadFloat4x4(&matrix);

    for (_uInt i = 0; i < EnumToInt(WORLDSTATE::POSITION); ++i)
        ParentMat.r[i] = XMVector3Normalize(ParentMat.r[i]);

    XMStoreFloat4x4(&m_ParentWorldMat, ParentMat);
}

void CTransform::ADD_Position(_vector vAddPos)
{
    auto vMovePosition = XMLoadFloat3(reinterpret_cast<_float3*>(m_WorldMat.m[3])) + vAddPos;
    XMStoreFloat3(reinterpret_cast<_float3*>(&m_WorldMat.m[3]), vMovePosition);
}

void CTransform::Turn(_vector vAxis, _float fTurnSpeed, _float fTimeDeleta)
{
    _vector vRight = GetRightVector();
    _vector vUp = GetUpVector();
    _vector vLook = GetLookVector();

    _matrix RoationMat = XMMatrixRotationAxis(vAxis, fTurnSpeed * fTimeDeleta);

    XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[0]), XMVector3TransformNormal(vRight, RoationMat));
    XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[1]), XMVector3TransformNormal(vUp, RoationMat));
    XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[2]), XMVector3TransformNormal(vLook, RoationMat));
}

void CTransform::LookAt(_vector vAt)
{
    _float3		vScale = GetScale();

    auto vPos = GetPosition();
    _vector		vLook = vAt - XMLoadFloat3(&vPos);
    _vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
    _vector		vUp = XMVector3Cross(vLook, vRight);

    XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[0]), XMVector3Normalize(vRight) * vScale.x);
    XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[1]), XMVector3Normalize(vUp) * vScale.y);
    XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMat.m[2]), XMVector3Normalize(vLook) * vScale.z);
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTransform* pInstance = new CTransform(pDevice, pContext);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : TRANSFORM");
    }

    return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
    CComponent* pInstance = new CTransform(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : TRANSFORM");
    }

    return pInstance;
}

void CTransform::Free()
{
    __super::Free();
}
