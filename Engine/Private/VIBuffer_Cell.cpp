#include "VIBuffer_Cell.h"

CVIBuffer_Cell::CVIBuffer_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Cell::CVIBuffer_Cell(const CVIBuffer_Cell& Prototype) :
    CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Cell::Initialize_Prototype(const _float3* pPoints)
{
    //정점 배열 개수
    m_iNumVertices = 3;
    //정점 하나의 크기
    m_iVertexStride = sizeof(VTX_COL);
    //정점 버퍼의 개수 설정
    m_iNumVertexBuffers = 1;

    //인덱스 배열 개수
    m_iNumIndices = 4;
    //인덱스 하나의 크기
    m_iIndexStride = sizeof(_ushort);

    //인덱스 포멧 비트단위임
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;

    //어떤 방식으로 그릴건지
    m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

#pragma region VERTEX BUFFER
    //정점 세팅
    VTX_COL* pVertices = new VTX_COL[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTX_COL) * m_iNumVertices);

    m_pVertices = new _float3[m_iNumVertices];
    ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);

    for(_uInt i = 0; i <3; ++ i)
        m_pVertices[i] = pVertices[i].vPosition = pPoints[i];

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
    _ushort* pIndices = new _ushort[m_iNumIndices] { 0, 1, 2, 0 };

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

HRESULT CVIBuffer_Cell::Initialize(void* pArg)
{
    return S_OK;
}

CVIBuffer_Cell* CVIBuffer_Cell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints)
{
	CVIBuffer_Cell* pInstance = new CVIBuffer_Cell(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pPoints)))
	{
		MSG_BOX("CREATE FAIL: VIBuffer_Cell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Cell::Clone(void* pArg)
{
	CVIBuffer_Cell* pInstance = new CVIBuffer_Cell(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("CLONE FAIL: VIBuffer_Cell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cell::Free()
{
    __super::Free();
}
