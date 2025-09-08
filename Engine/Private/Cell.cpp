#include "Cell.h"

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

CCell* CCell::Create(_uInt iCellIndex, _uInt iCellProperity, const _float3* vPoints)
{
    CCell* pCell = new CCell();
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
