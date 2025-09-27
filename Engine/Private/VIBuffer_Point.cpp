#include "VIBuffer_Point.h"

CVIBuffer_Point::CVIBuffer_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Point::CVIBuffer_Point(const CVIBuffer_Point& rhs) :
    CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Point::Initialize_Prototype()
{
    //정점 배열 개수
    m_iNumVertices = 1;
    //정점 하나의 크기
    m_iVertexStride = sizeof(VTX_POINT);
    //정점 버퍼의 개수 설정
    m_iNumVertexBuffers = 1;

    //인덱스 배열 개수
    m_iNumIndices = 6;
    //인덱스 하나의 크기
    m_iIndexStride = sizeof(_ushort);

    //인덱스 포멧 비트단위임
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;

    //어떤 방식으로 그릴건지
    m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VERTEX BUFFER
    //정점 세팅
    VTX_POINT* pVertices = new VTX_POINT[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTX_POINT) * m_iNumVertices);

    m_pVertices = new _float3[m_iNumVertices];
    ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);

    m_pVertices[0] = pVertices[0].vPosition = _float3(0.f, 0.f, 0.f);

    //버퍼 세팅
    D3D11_BUFFER_DESC			m_VertexDesc = {};
    m_VertexDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    m_VertexDesc.Usage = D3D11_USAGE_DEFAULT;
    m_VertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_VertexDesc.CPUAccessFlags = 0;
    m_VertexDesc.MiscFlags = 0;
    m_VertexDesc.StructureByteStride = m_iVertexStride;

    D3D11_SUBRESOURCE_DATA	InitialVBData{};
    InitialVBData.pSysMem = pVertices;

    if (FAILED(m_pDevice->CreateBuffer(&m_VertexDesc, &InitialVBData, &m_pVertexBuffer)))
        return E_FAIL;
#pragma endregion

#pragma region INDEX BUFFER
    //인덱스 셑팅
    _ushort* pIndices = new _ushort[m_iNumIndices]
    {
       0, 1, 2,
       0, 2, 3
    };

    //인덱스 버퍼 세팅
    D3D11_BUFFER_DESC			IndexDesc = {};
    IndexDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
    IndexDesc.Usage = D3D11_USAGE_DEFAULT;
    IndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    IndexDesc.CPUAccessFlags = 0;
    IndexDesc.MiscFlags = 0;
    IndexDesc.StructureByteStride = m_iIndexStride;

    D3D11_SUBRESOURCE_DATA	InitialIBData{};
    InitialIBData.pSysMem = pIndices;

    if (FAILED(m_pDevice->CreateBuffer(&IndexDesc, &InitialIBData, &m_pIndexBuffer)))
        return E_FAIL;
#pragma endregion

    Safe_Delete_Array(pVertices);
    Safe_Delete_Array(pIndices);

    return S_OK;
}

HRESULT CVIBuffer_Point::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_Point::Render_VIBuffer()
{
    __super::Render_VIBuffer();
}

CVIBuffer_Point* CVIBuffer_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVIBuffer_Point* pVIBuffer_Point = new CVIBuffer_Point(pDevice, pContext);
    if (FAILED(pVIBuffer_Point->Initialize_Prototype()))
    {
        Safe_Release(pVIBuffer_Point);
        MSG_BOX("CREATE FAIL : VIBUFFER POINT");
    }

    return pVIBuffer_Point;
}

CComponent* CVIBuffer_Point::Clone(void* pArg)
{
    CVIBuffer_Point* pVIBuffer_Point = new CVIBuffer_Point(*this);
    if (FAILED(pVIBuffer_Point->Initialize(pArg)))
    {
        Safe_Release(pVIBuffer_Point);
        MSG_BOX("CLONE FAIL : VIBUFFER POINT");
    }

    return pVIBuffer_Point;
}

void CVIBuffer_Point::Free()
{
    __super::Free();
}
