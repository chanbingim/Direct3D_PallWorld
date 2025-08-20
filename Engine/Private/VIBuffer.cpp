#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs) :
    CComponent(rhs),
    m_pVertexBuffer(rhs.m_pVertexBuffer),
    m_pIndexBuffer(rhs.m_pIndexBuffer),
    m_pVertices(rhs.m_pVertices),
    m_iNumIndices(rhs.m_iNumIndices),
    m_iVertexStride(rhs.m_iVertexStride),
    m_iIndexStride(rhs.m_iIndexStride),
    m_iNumVertices(rhs.m_iNumVertices),
    m_iNumVertexBuffers (rhs.m_iNumVertexBuffers),
    m_eIndexFormat(rhs.m_eIndexFormat),
    m_ePrimitive(rhs.m_ePrimitive)
{
    Safe_AddRef(m_pVertexBuffer);
    Safe_AddRef(m_pIndexBuffer);
    m_isCloned = true;
}

HRESULT CVIBuffer::Initialize_Prototype(D3D11_BUFFER_DESC* vtxDesc, D3D11_BUFFER_DESC* idxDesc)
{

    return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer::Render_VIBuffer()
{
    ID3D11Buffer* VertexBuffers[] = {
            m_pVertexBuffer,
    };

    _uInt		VertexStrides[] = {
        m_iVertexStride,
    };

    _uInt		Offsets[] = {
        0,
    };

    //이게 버택스 스트림 같은 역할을 하는 함수인거같다.
    //설마 여기 상수로 못때려박아?
    m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, &m_pVertexBuffer, VertexStrides, Offsets);

    m_pContext->IASetPrimitiveTopology(m_ePrimitive);

    // 여기서 DX9에서 했던 Format이 16비트 짜리냐 32비트짜리냐를 설정해준다.
    // 그다음 인덱스 버퍼를 설정하는 함수
    m_pContext->IASetIndexBuffer(m_pIndexBuffer, m_eIndexFormat, 0);

    //인덱스를 통해서 객체 그리기 함수
    m_pContext->DrawIndexed(m_iNumIndices, 0, 0);
}

_bool CVIBuffer::IsPicking(CTransform* pTransform, _float3* pOut)
{
    return _bool();
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
        Safe_Delete_Array(m_pVertices);
    }
}
