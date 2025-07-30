#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs) :
    CComponent(rhs),
    m_pVertexBuffer(rhs.m_pVertexBuffer),
    m_pIndexBuffer(rhs.m_pIndexBuffer),
    m_Vertices(rhs.m_Vertices),
    m_NumIndices(rhs.m_NumIndices),
    m_NumVertices(rhs.m_NumVertices)
{
    Safe_AddRef(m_pVertexBuffer);
    Safe_AddRef(m_pIndexBuffer);
    m_isCloned = true;
}

HRESULT CVIBuffer::Initialize_Prototype(D3D11_BUFFER_DESC* vtxDesc, D3D11_BUFFER_DESC* idxDesc)
{
    // 리소스를 초기화 하기위한 데이터 저장
    // pSysMem      : 정점 배열을 넣을 포인터
    // 
    // 아래 두개는 써봐야 알것같다.
    // SysMemPitch  : 텍스처의 한줄 시작부터 다음줄 까지의 거리(바이트)
    // SysMemSlicePitch : 깊이 수준의 시작부터 다음수준까지의 거리(바이트)

    D3D11_SUBRESOURCE_DATA VtxData;
    ZeroMemory(&VtxData, sizeof(D3D11_SUBRESOURCE_DATA));
    VtxData.pSysMem = m_Vertices;

    if (FAILED(m_pDevice->CreateBuffer(vtxDesc, &VtxData, &m_pVertexBuffer)))
        return E_FAIL;

    D3D11_SUBRESOURCE_DATA IdxData;
    ZeroMemory(&IdxData, sizeof(D3D11_SUBRESOURCE_DATA));
    IdxData.pSysMem = m_Indices;

    if (FAILED(m_pDevice->CreateBuffer(idxDesc, &IdxData, &m_pIndexBuffer)))
        return E_FAIL;

    return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer::Render_VIBuffer()
{
    UINT stride = sizeof(VTX_TEX);
    UINT offset = 0;

    //이게 버택스 스트림 같은 역할을 하는 함수인거같다.
    //설마 여기 상수로 못때려박아?
    m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 여기서 DX9에서 했던 Format이 16비트 짜리냐 32비트짜리냐를 설정해준다.
    // 그다음 인덱스 버퍼를 설정하는 함수
    m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    //인덱스를 통해서 객체 그리기 함수
    m_pContext->DrawIndexed(m_NumIndices, 0, 0);
}

CComponent* CVIBuffer::Clone(void* pArg)
{
    return nullptr;
}

void CVIBuffer::Free()
{
    __super::Free();

    Safe_Release(m_pVertexBuffer);
    Safe_Release(m_pIndexBuffer);

    if (!m_isCloned)
    {
        Safe_Delete_Array(m_Vertices);
        Safe_Delete_Array(m_Indices);
    }
}
