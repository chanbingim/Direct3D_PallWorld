#include "VIBuffer_Instance.h"

CVIBuffer_Instance::CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Instance::CVIBuffer_Instance(const CVIBuffer_Instance& Prototype) :
    CVIBuffer(Prototype)
	, m_InstanceBufferDesc(Prototype.m_InstanceBufferDesc)
	, m_InstanceInitialDesc{ Prototype.m_InstanceInitialDesc }
	, m_iInstanceStride{ Prototype.m_iInstanceStride }
	, m_iNumInstance{ Prototype.m_iNumInstance }
	, m_iNumIndexPerInstance{ Prototype.m_iNumIndexPerInstance }
	, m_vBoxSize{ Prototype.m_vBoxSize }
{
}

HRESULT CVIBuffer_Instance::Initialize_Prototype(const INSTANCE_DESC* pInstanceDesc)
{ 
	

    return S_OK;
}

HRESULT CVIBuffer_Instance::Initialize(void* pArg)
{


    return S_OK;
}

void CVIBuffer_Instance::Render_VIBuffer()
{
	Bind_Resource();
	m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_iNumInstance, 0, 0, 0);

}

void CVIBuffer_Instance::Lock(D3D11_MAPPED_SUBRESOURCE* pSubResource)
{
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, pSubResource);
}

void CVIBuffer_Instance::UnLock()
{
	m_pContext->Unmap(m_pVBInstance, 0);
}

_float3 CVIBuffer_Instance::GetInstanceModelBoundSize()
{
	return m_vBoxSize;
}

void CVIBuffer_Instance::Bind_Resource()
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
	m_pContext->IASetIndexBuffer(m_pIndexBuffer, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitive);
}

CComponent* CVIBuffer_Instance::Clone(void* pArg)
{
    return nullptr;
}

void CVIBuffer_Instance::Free()
{
    __super::Free();

    Safe_Release(m_pVBInstance);
}
