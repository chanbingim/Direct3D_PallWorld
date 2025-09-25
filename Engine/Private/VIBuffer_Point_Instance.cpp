#include "VIBuffer_Point_Instance.h"

#include "GameInstance.h"

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CVIBuffer_Instance(pDevice, pContext)
{
}

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& Prototype) :
	CVIBuffer_Instance(Prototype)
{
}

HRESULT CVIBuffer_Point_Instance::Initialize_Prototype(const INSTANCE_DESC* pInstanceDesc)
{
	m_iNumVertexBuffers = 2;
	m_iNumVertices = 1;
	m_iVertexStride = sizeof(VTX_POINT);

	m_iNumIndices = 0;
	m_iIndexStride = 0;

	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VERTEX_BUFFER
	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTX_POINT* pVertices = new VTX_POINT[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTX_POINT) * m_iNumVertices);

	m_pVertices = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);
	m_pVertices[0] = pVertices[0].vPosition = _float3(0.0f, 0.0f, 0.f);

	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	InitialVBData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pIndexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INSTANCE_BUFFER
	const INSTANCE_DESC* pDesc = static_cast<const INSTANCE_DESC*>(pInstanceDesc);
	m_iInstanceStride = sizeof(VTX_INSTANCE_DEFAULT_DESC);
	m_iNumIndexPerInstance = 4;

	m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;
	m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_InstanceBufferDesc.MiscFlags = 0;

	m_pInstanceVertices = new VTX_INSTANCE_DEFAULT_DESC[m_iNumInstance];
	ZeroMemory(m_pInstanceVertices, sizeof(VTX_INSTANCE_DEFAULT_DESC) * m_iNumInstance);
	m_InstanceInitialDesc.pSysMem = m_pInstanceVertices;

#pragma endregion

    return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Initialize(void* pArg)
{
	if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InstanceInitialDesc, &m_pVBInstance)))
		return E_FAIL;

    return S_OK;
}

void CVIBuffer_Point_Instance::Render_VIBuffer()
{
	Bind_Resource();
	m_pContext->DrawInstanced(m_iVertexStride, m_iNumInstance, 0, 0);
}

void CVIBuffer_Point_Instance::Bind_Resource()
{
	ID3D11Buffer* VertexBuffers[] = {
	   m_pVertexBuffer,
	   m_pVBInstance,

	};

	_uInt		VertexStrides[] = {
		m_iVertexStride,
		m_iInstanceStride,
	};

	_uInt		Offsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, VertexBuffers, VertexStrides, Offsets);
	m_pContext->IASetPrimitiveTopology(m_ePrimitive);
}

CVIBuffer_Point_Instance* CVIBuffer_Point_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pInstanceDesc)
{
	CVIBuffer_Point_Instance* pVIBuffer_Point_Instacne = new CVIBuffer_Point_Instance(pDevice, pContext);
	if (FAILED(pVIBuffer_Point_Instacne->Initialize_Prototype(pInstanceDesc)))
	{
		Safe_Release(pVIBuffer_Point_Instacne);
		MSG_BOX("CREATE FAIL : POINT INSTANCE BUFFER");
	}
    return pVIBuffer_Point_Instacne;
}

CComponent* CVIBuffer_Point_Instance::Clone(void* pArg)
{
	CVIBuffer_Point_Instance* pVIBuffer_Point_Instacne = new CVIBuffer_Point_Instance(*this);
	if (FAILED(pVIBuffer_Point_Instacne->Initialize(pArg)))
	{
		Safe_Release(pVIBuffer_Point_Instacne);
		MSG_BOX("CLONE FAIL : POINT INSTANCE BUFFER");
	}
	return pVIBuffer_Point_Instacne;
}

void CVIBuffer_Point_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete(m_pInstanceVertices);
	}
}
