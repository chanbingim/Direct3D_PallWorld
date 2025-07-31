#include "VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& rhs) :
    CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
    //정점 배열 개수
    m_iNumVertices = 4;
    //정점 하나의 크기
    m_iVertexStride = sizeof(VTX_TEX);
    //정점 버퍼의 개수 설정
    m_iNumVertexBuffers = 1;

    //인덱스 배열 개수
    m_iNumIndices = 6;
    //인덱스 하나의 크기
    m_iIndexStride = sizeof(_ushort);

    //인덱스 포멧 비트단위임
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;

    //어떤 방식으로 그릴건지
    m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX BUFFER
    //정점 세팅
    VTX_TEX* pVertices = new VTX_TEX[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTX_TEX) * m_iNumVertices);

    m_pVertices = new _float3[m_iNumVertices];
    ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);

    m_pVertices[0] = pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
    pVertices[0].vTexcoord = _float2(0.0f, 0.0f);

    m_pVertices[1] = pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
    pVertices[1].vTexcoord = _float2(1.0f, 0.0f);

    m_pVertices[2] = pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
    pVertices[2].vTexcoord = _float2(1.0f, 1.0f);

    m_pVertices[3] = pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
    pVertices[3].vTexcoord = _float2(0.0f, 1.0f);

    //버퍼 세팅
    D3D11_BUFFER_DESC			m_VertexDesc = {};
    m_VertexDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    m_VertexDesc.Usage =     D3D11_USAGE_DEFAULT;
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
    IndexDesc.Usage =     D3D11_USAGE_DEFAULT;
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

HRESULT CVIBuffer_Rect::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_Rect::Render_VIBuffer()
{
    __super::Render_VIBuffer();
}

CVIBuffer_Rect* CVIBuffer_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVIBuffer_Rect* pVIBuffer_Rect = new CVIBuffer_Rect(pDevice, pContext);
    if (FAILED(pVIBuffer_Rect->Initialize_Prototype()))
    {
        Safe_Release(pVIBuffer_Rect);
        MSG_BOX("CREATE FAIL : VIBUFFER RECT");
    }

    return pVIBuffer_Rect;
}

CComponent* CVIBuffer_Rect::Clone(void* pArg)
{
    CVIBuffer_Rect* pVIBuffer_Rect = new CVIBuffer_Rect(*this);
    if (FAILED(pVIBuffer_Rect->Initialize(pArg)))
    {
        Safe_Release(pVIBuffer_Rect);
        MSG_BOX("CLONE FAIL : VIBUFFER RECT");
    }

    return pVIBuffer_Rect;
}

void CVIBuffer_Rect::Free()
{
    __super::Free();
}
