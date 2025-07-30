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
    //정점 배열 세팅 및 정점 구조체 세팅
    m_NumVertices = 4;
    m_Vertices = new VTX_TEX[m_NumVertices]
    {
        /*{_float3(-0.5f, 0.5f, 0.f), _float4(1.0f, 0.0f, 0.0f, 1.0f)},
        {_float3(0.5f, 0.5f, 0.f), _float4(1.0f, 0.0f, 0.0f, 1.0f)},
        {_float3(0.5f, -0.5f, 0.f), _float4(1.0f, 0.0f, 0.0f, 1.0f)},
        {_float3(-0.5f, -0.5f, 0.f), _float4(1.0f, 0.0f, 0.0f, 1.0f)},*/

        {_float3(-0.5f, 0.5f, 0.f), _float2(0.0f, 0.0f)},
        {_float3(0.5f, 0.5f, 0.f), _float2(1.0f, 0.0f)},
        {_float3(0.5f, -0.5f, 0.f), _float2(1.0f, 1.0f)},
        {_float3(-0.5f, -0.5f, 0.f), _float2(0.0f, 1.0f)},
    };

    D3D11_BUFFER_DESC			m_VertexDesc = {};
    m_VertexDesc.ByteWidth = sizeof(VTX_TEX) * m_NumVertices;
    m_VertexDesc.Usage = D3D11_USAGE_IMMUTABLE;
    m_VertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_VertexDesc.CPUAccessFlags = 0;
    m_VertexDesc.MiscFlags = 0;
    m_VertexDesc.StructureByteStride = 0;

    m_NumIndices = 6;
    //인덱스 배열 세팅 및 인덱스 구조체 세팅
    m_Indices = new _uInt[m_NumIndices]
    {
       0, 1, 2,
       0, 2, 3
    };

    D3D11_BUFFER_DESC			m_IndexDesc = {};
    m_IndexDesc.ByteWidth = sizeof(_uInt) * m_NumIndices;
    m_IndexDesc.Usage = D3D11_USAGE_IMMUTABLE;
    m_IndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_IndexDesc.CPUAccessFlags = 0;
    m_IndexDesc.MiscFlags = 0;
    m_IndexDesc.StructureByteStride = 0;

    if (FAILED(__super::Initialize_Prototype(&m_VertexDesc, &m_IndexDesc)))
        return E_FAIL;

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
