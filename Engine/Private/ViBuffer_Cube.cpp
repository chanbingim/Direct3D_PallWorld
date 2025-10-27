#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& rhs) :
    CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
	//정점 배열 개수
    m_iNumVertices = 8;
    //정점 하나의 크기
    m_iVertexStride = sizeof(VTX_CUBE);
    //정점 버퍼의 개수 설정
    m_iNumVertexBuffers = 1;

    //인덱스 배열 개수
    m_iNumIndices = 36;
    //인덱스 하나의 크기
    m_iIndexStride = sizeof(_ushort);

    //인덱스 포멧 비트단위임
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;

    //어떤 방식으로 그릴건지
    m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX BUFFER
    //정점 세팅
    VTX_CUBE* pVertices = new VTX_CUBE[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTX_CUBE) * m_iNumVertices);

    m_pVertices = new _float3[m_iNumVertices];
    ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);

    m_pVertices[0] = pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
    pVertices[0].vTexcoord = m_pVertices[0];

    m_pVertices[1] = pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
    pVertices[1].vTexcoord = m_pVertices[1];

    m_pVertices[2] = pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
    pVertices[2].vTexcoord = m_pVertices[2];

    m_pVertices[3] = pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
    pVertices[3].vTexcoord = m_pVertices[3];

    m_pVertices[4] = pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
    pVertices[4].vTexcoord = m_pVertices[4];

    m_pVertices[5] = pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
    pVertices[5].vTexcoord = m_pVertices[5];

    m_pVertices[6] = pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
    pVertices[6].vTexcoord = m_pVertices[6];

    m_pVertices[7] = pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
    pVertices[7].vTexcoord = m_pVertices[7];

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
    _ushort* pIndices = new _ushort[m_iNumIndices];
    /* + x */
    pIndices[0] = 1; pIndices[1] = 5; pIndices[2] = 6;
    pIndices[3] = 1; pIndices[4] = 6; pIndices[5] = 2;

    /* - x */
    pIndices[6] = 4; pIndices[7] = 0; pIndices[8] = 3;
    pIndices[9] = 4; pIndices[10] = 3; pIndices[11] = 7;

    /* + y */
    pIndices[12] = 4; pIndices[13] = 5; pIndices[14] = 1;
    pIndices[15] = 4; pIndices[16] = 1; pIndices[17] = 0;

    /* - y */
    pIndices[18] = 3; pIndices[19] = 2; pIndices[20] = 6;
    pIndices[21] = 3; pIndices[22] = 6; pIndices[23] = 7;

    /* + z */
    pIndices[24] = 5; pIndices[25] = 4; pIndices[26] = 7;
    pIndices[27] = 5; pIndices[28] = 7; pIndices[29] = 6;

    /* - z */
    pIndices[30] = 0; pIndices[31] = 1; pIndices[32] = 2;
    pIndices[33] = 0; pIndices[34] = 2; pIndices[35] = 3;

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

HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}

void CVIBuffer_Cube::Render_VIBuffer()
{
    __super::Render_VIBuffer();
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVIBuffer_Cube* pCubeBuffer = new CVIBuffer_Cube(pDevice, pContext);
    if (FAILED(pCubeBuffer->Initialize_Prototype()))
    {
        Safe_Release(pCubeBuffer);
        MSG_BOX("CREATE FAIL : CUBE BUFFER");
    }
    return pCubeBuffer;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
    CVIBuffer_Cube* pCubeBuffer = new CVIBuffer_Cube(*this);
    if (FAILED(pCubeBuffer->Initialize(pArg)))
    {
        Safe_Release(pCubeBuffer);
        MSG_BOX("CLONE FAIL : CUBE BUFFER");
    }
    return pCubeBuffer;
}

void CVIBuffer_Cube::Free()
{
    __super::Free();
}
