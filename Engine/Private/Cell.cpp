#include "Cell.h"
#include "VIBuffer_Cell.h"

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
    for (_uInt i = 0; i < ENUM_CLASS(NAVI_POINT::END); ++i)
    {
        _uInt LeftIndex = i + 1;
        if (ENUM_CLASS(NAVI_POINT::END) <= LeftIndex)
            LeftIndex = 0;

        XMStoreFloat3(&vLines[i], XMLoadFloat3(&m_vTirPoints[i + 1]) - XMLoadFloat3(&m_vTirPoints[i]));
        m_vTirNormals[i] = _float3(vLines[i].z * -1.f, 0.f, vLines[i].x);
    }
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
            return true;

        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)]), vDestPoint))
            return true;
    }
    if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::B)]), vSourPoint))
    {
        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)]), vDestPoint))
            return true;

        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::A)]), vDestPoint))
            return true;
    }

    if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::C)]), vSourPoint))
    {
        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::A)]), vDestPoint))
            return true;

        if (true == XMVector3Equal(XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(NAVI_POINT::B)]), vDestPoint))
            return true;
    }
    return false;
}

void CCell::SetNeighbor(NAVI_LINE eLine, _Int iIndex)
{
    m_NeighborIndices[ENUM_CLASS(eLine)] = iIndex;
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

void CCell::Render()
{
    m_pVIBuffer->Render_VIBuffer();
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
}
