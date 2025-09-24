#include "Cell.h"
#include "VIBuffer_Cell.h"
#include "NaviMeshPaserStruct.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(_uInt iCellIndex, _uInt iCellProperity, const _float3* vPoints)
{
    m_iCellIndex = iCellIndex;
    m_iCellProperity = iCellProperity;

    memcpy(m_vTirPoints, vPoints, sizeof(_float3) * ENUM_CLASS(NAVI_POINT::END));

    _float3			vLines[ENUM_CLASS(NAVI_LINE::END)] = {};
    _vector         total = {};
    for (_uInt i = 0; i < ENUM_CLASS(NAVI_POINT::END); ++i)
    {
        _uInt LeftIndex = i + 1;
        if (ENUM_CLASS(NAVI_POINT::END) <= LeftIndex)
            LeftIndex = 0;

        XMStoreFloat3(&vLines[i], XMLoadFloat3(&m_vTirPoints[LeftIndex]) - XMLoadFloat3(&m_vTirPoints[i]));
        m_vTirNormals[i] = _float3(vLines[i].z * -1.f, 0.f, vLines[i].x);

        total += XMLoadFloat3(&m_vTirPoints[i]);
    }

    XMStoreFloat3(&m_fCenter, total / 3.0f);

#ifdef _DEBUG
    m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, vPoints);
    if (nullptr == m_pVIBuffer)
        return E_FAIL;
#endif // _DEBUG
    XMStoreFloat4(&m_vPlane,
        XMPlaneFromPoints(XMLoadFloat3(&m_vTirPoints[0]), XMLoadFloat3(&m_vTirPoints[1]), XMLoadFloat3(&m_vTirPoints[2])));

    return S_OK;
}

_bool CCell::IsCellIn(_vector vPos, _Int* pNeighborIndex)
{
    for (_uInt i = 0; i < ENUM_CLASS(NAVI_LINE::END); ++i)
    {
        _vector vDir =  XMVector3Normalize(vPos - XMLoadFloat3(&m_vTirPoints[i]));
        _vector vNormal = XMLoadFloat3(&m_vTirNormals[i]);

        if (0.f < XMVectorGetX(XMVector3Dot(vDir, vNormal)))
        {
            *pNeighborIndex = m_NeighborIndices[i];
            return false;
        }
    }
    return true;
}

_bool CCell::Compare(_vector vSourPoint, _vector vDestPoint)
{
    if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::A)]), vSourPoint))
    {
        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::B)]), vDestPoint))
        {
            _uInt iIndex = ENUM_CLASS(NAVI_LINE::AB);
            m_Portals[iIndex].vLeftPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::A)];
            m_Portals[iIndex].vRightPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::B)];
            return true;
        }
        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)]), vDestPoint))
        {
            _uInt iIndex = ENUM_CLASS(NAVI_LINE::CA);
            m_Portals[iIndex].vLeftPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)];
            m_Portals[iIndex].vRightPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::A)];
            return true;
        }
    }
    if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::B)]), vSourPoint))
    {
        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)]), vDestPoint))
        {
            _uInt iIndex = ENUM_CLASS(NAVI_LINE::BC);
            m_Portals[iIndex].vLeftPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::B)];
            m_Portals[iIndex].vRightPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)];
            return true;
        }

        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::A)]), vDestPoint))
        {
            _uInt iIndex = ENUM_CLASS(NAVI_LINE::AB);
            m_Portals[iIndex].vLeftPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::A)];
            m_Portals[iIndex].vRightPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::B)];
            return true;
        }
    }

    if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)]), vSourPoint))
    {
        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::A)]), vDestPoint))
        {
            _uInt iIndex = ENUM_CLASS(NAVI_LINE::CA);
            m_Portals[iIndex].vLeftPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)];
            m_Portals[iIndex].vRightPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::A)];
            return true;
        }

        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::B)]), vDestPoint))
        {
            _uInt iIndex = ENUM_CLASS(NAVI_LINE::BC);
            m_Portals[iIndex].vLeftPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::B)];
            m_Portals[iIndex].vRightPoint = m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)];
            return true;
        }
    }
    return false;
}

void CCell::SetNeighbor(NAVI_LINE eLine, _Int iIndex)
{
    m_NeighborIndices[ENUM_CLASS(eLine)] = iIndex;
    m_Portals[ENUM_CLASS(eLine)].iCellAIndex = m_iCellIndex;
    m_Portals[ENUM_CLASS(eLine)].iCellBIndex = iIndex;
}

_bool CCell::IsMoveAble()
{
    switch (m_eType)
    {
    case CELL_TYPE::BUILD:
        return false;
    }

    return true;
}

_float CCell::ComputeHeight(_vector vPos)
{
    return (-m_vPlane.x * XMVectorGetX(vPos) - m_vPlane.z * XMVectorGetZ(vPos) - m_vPlane.w) / m_vPlane.y;
}

_vector CCell::GetCellPoint(NAVI_POINT eType)
{
    return XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(eType)]);
}

_vector CCell::GetCellCenterPoint()
{
    return XMLoadFloat3(&m_fCenter);
}

void CCell::GetNeighborIndex(_Int* pNeighborIndex)
{
    pNeighborIndex[0] = m_NeighborIndices[ENUM_CLASS(NAVI_LINE::AB)];
    pNeighborIndex[1] = m_NeighborIndices[ENUM_CLASS(NAVI_LINE::BC)];
    pNeighborIndex[2] = m_NeighborIndices[ENUM_CLASS(NAVI_LINE::CA)];
}

const PORTAL_DESC* CCell::GetPortal(_Int iNeighborIndex)
{
    for (_uInt i = 0; i < ENUM_CLASS(NAVI_LINE::END); ++i)
    {
        if (iNeighborIndex == m_NeighborIndices[i])
            return &m_Portals[i];
    }
    return nullptr;
}

#ifdef _DEBUG
void CCell::Render()
{
    m_pVIBuffer->Render_VIBuffer();
}
#endif

void CCell::Export(void* pArg)
{
    CELL_SAVE_STRCUT* CellSaveData = static_cast<CELL_SAVE_STRCUT*>(pArg);
    CellSaveData->iProperity = m_iCellProperity;
    CellSaveData->vPointA = m_vTirPoints[ENUM_CLASS(NAVI_POINT::A)];
    CellSaveData->vPointB = m_vTirPoints[ENUM_CLASS(NAVI_POINT::B)];
    CellSaveData->vPointC = m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)];
}

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt iCellIndex, _uInt iCellProperity, const _float3* vPoints)
{
    CCell* pCell = new CCell(pDevice, pContext);
    if (FAILED(pCell->Initialize(iCellIndex, iCellProperity, vPoints)))
    {
        Safe_Release(pCell);
        MSG_BOX("CREATE FAIL : CELL");
    }
    return pCell;
}

void CCell::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pVIBuffer);
}
