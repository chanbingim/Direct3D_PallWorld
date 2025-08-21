#include "Picking.h"

#include "GameInstance.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext),
    m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

void CPicking::Update()
{
    //메인 카메라의 원근, 직교 상태에 따라  시작지점을 다르게 세팅함
   // 
   //공통적으로하는건 카메라 위치통해서 view와 World 갱신
    _vector RayPos = {0.f, 0.f, 0.f, 0.f};

    // View스페이스 좌표에서 View스페이스의 역행렬을 곱해서 월드변환을 한다.
    _matrix InvViewMat = XMLoadFloat4x4(&m_pGameInstance->GetInvMatrix(MAT_STATE::VIEW));

    //Ray의 시작위치는 카메라의 위치부터
    _vector WorldPos = XMVector3TransformCoord(RayPos, InvViewMat);
    XMStoreFloat3(&m_RayPos[ENUM_CLASS(RAY::WORLD)], WorldPos);

    // z깊이는 0으로 두고 위치 값을 받을거기때문에 w 성분은 1로 채운다.
    if (GAMEMODE::EDITOR == m_eMode)
    {
        _vector NDCPos = {  m_EditorMousePos.x / (m_EditorSize.x * 0.5f) - 1.f,
                            m_EditorMousePos.y / -(m_EditorSize.y * 0.5f) + 1.f, 0.f, 1.f };

        // 투영행렬의 역행렬을 곱해 View스페이스로 변환한다.
        _matrix InvProjMat = XMLoadFloat4x4(&m_pGameInstance->GetInvMatrix(MAT_STATE::PROJECTION));
        _vector ViewSpacePos = XMVector3TransformCoord(NDCPos, InvProjMat);

        //RayDir은 마우스 피킹위치
        _vector WorldDir = XMVector3TransformNormal(ViewSpacePos, InvViewMat);
        WorldDir = XMVector3Normalize(WorldDir);

        XMStoreFloat3(&m_RayDir[ENUM_CLASS(RAY::WORLD)], WorldDir);
    }
    else
    {
        m_RayDir[ENUM_CLASS(RAY::WORLD)] = m_pGameInstance->GetRayPos();
    }
}

void CPicking::SetEditor_MousePos(const _float3& vPos)
{
    m_EditorMousePos = vPos;
}

void CPicking::SetEditor_Frame(const _float2& vSize)
{
    m_EditorSize = vSize;
}

void CPicking::Change_Mode(GAMEMODE eType)
{
    m_eMode = eType;
}

_bool CPicking::RayPicking(_vector vRayOrizin, _vector vRayDir, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
    _float      fDist{};
    _bool       IsCall = false;

    IsCall = TriangleTests::Intersects(vRayOrizin, vRayDir, XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);

    if (IsCall)
    {
        XMStoreFloat3(pOut, (vRayOrizin + vRayDir * fDist));
    }

    return IsCall;
}

//월드상의 폴리곤 단위의 정점 3개
_bool CPicking::Picking_InWorld(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
    _float      fDist{};
    _bool       IsCall = false;

    _vector WorldPos = XMLoadFloat3(&m_RayPos[ENUM_CLASS(RAY::WORLD)]);
    _vector WorldDir = XMLoadFloat3(&m_RayDir[ENUM_CLASS(RAY::WORLD)]);
    IsCall = TriangleTests::Intersects(WorldPos, WorldDir, XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);

    if (IsCall)
    {
        XMStoreFloat3(pOut, (WorldPos + WorldDir * fDist));
    }

    return IsCall;
}

//로컬상의 폴리곤 단위의 정점 3개
_bool CPicking::Picking_InLocal(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
    _float      fDist{};
    _bool       IsCall = false;

    _vector WorldPos = XMLoadFloat3(&m_RayPos[ENUM_CLASS(RAY::LOCAL)]);
    _vector WorldDir = XMLoadFloat3(&m_RayDir[ENUM_CLASS(RAY::LOCAL)]);
    IsCall = TriangleTests::Intersects(WorldPos, WorldDir, XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);

    if (IsCall)
    {
        XMStoreFloat3(pOut, (WorldPos + WorldDir * fDist));
    }

    return IsCall;
}

void CPicking::Compute_LocalRay(const _matrix* InvWorldMatrix)
{
    _vector WorldPos = XMLoadFloat3(&m_RayPos[ENUM_CLASS(RAY::WORLD)]);
    _vector WorldDir = XMLoadFloat3(&m_RayDir[ENUM_CLASS(RAY::WORLD)]);

    WorldPos = XMVector3TransformCoord(WorldPos, *InvWorldMatrix);
    
    WorldDir = XMVector3TransformNormal(WorldDir, *InvWorldMatrix);
    WorldDir = XMVector3Normalize(WorldDir);

    XMStoreFloat3(&m_RayPos[ENUM_CLASS(RAY::LOCAL)], WorldPos);
    XMStoreFloat3(&m_RayDir[ENUM_CLASS(RAY::LOCAL)], WorldDir);
}

const _float3& CPicking::GetRayPos(RAY eType)
{
    return m_RayPos[ENUM_CLASS(eType)];
}

const _float3& CPicking::GetRayDir(RAY eType)
{
    return m_RayDir[ENUM_CLASS(eType)];
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPicking* pPicking = new CPicking(pDevice, pContext);
    return pPicking;
}

void CPicking::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}
