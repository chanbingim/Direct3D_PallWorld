#include "VIBuffer_Model_Instance.h"

#include "GameInstance.h"

CVIBuffer_Model_Instance::CVIBuffer_Model_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CVIBuffer_Instance(pDevice, pContext)
{
}

CVIBuffer_Model_Instance::CVIBuffer_Model_Instance(const CVIBuffer_Model_Instance& Prototype) :
	CVIBuffer_Instance(Prototype),
	m_pModel(Prototype.m_pModel),
	m_pInstanceVertices(Prototype.m_pInstanceVertices)
{
	Safe_AddRef(m_pModel);
}

HRESULT CVIBuffer_Model_Instance::Initialize_Prototype(const INSTANCE_DESC* pInstanceDesc)
{
	const MODEL_INSTANCE_DESC* pDesc = static_cast<const MODEL_INSTANCE_DESC*>(pInstanceDesc);
	m_pModel = CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM,
							  pDesc->pModelFilePath, 
							  pDesc->PreModelMat,
							  pDesc->RetargetFile,
							  pDesc->iLayerCount);

	if (nullptr == m_pModel)
		return E_FAIL;

	m_iNumVertexBuffers = 2;
	m_ePrimitive = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_iNumInstance = pDesc->iNumInstance;
	m_iInstanceStride = sizeof(VTX_INSTANCE_DEFAULT_DESC);
	m_iNumIndexPerInstance = 6;

	m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;
	m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_InstanceBufferDesc.MiscFlags = 0;

	m_pInstanceVertices = new VTX_INSTANCE_DEFAULT_DESC[m_iNumInstance];
	ZeroMemory(m_pInstanceVertices, sizeof(VTX_INSTANCE_DEFAULT_DESC) * m_iNumInstance);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float			fScale = m_pGameInstance->Random(pDesc->vSize.x, pDesc->vSize.y);

		m_pInstanceVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
		m_pInstanceVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
		m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		m_pInstanceVertices[i].vPosition = _float4(
			m_pGameInstance->Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f),
			m_pGameInstance->Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f),
			m_pGameInstance->Random(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f),
			1.f);
	}

	m_InstanceInitialDesc.pSysMem = m_pInstanceVertices;

	return S_OK;
}

HRESULT CVIBuffer_Model_Instance::Initialize(void* pArg)
{
	if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InstanceInitialDesc, &m_pVBInstance)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Model_Instance::Render_VIBuffer(_uInt iIndex)
{
	Bind_Resource(iIndex);
	m_pContext->DrawIndexedInstanced(m_iNumIndices, m_iNumInstance, 0, 0, 0);
}

HRESULT CVIBuffer_Model_Instance::GetMeshResource(_uInt iMeshIndex, aiTextureType eType, _uInt iTextureIndex, ID3D11ShaderResourceView** ppOut)
{
	return m_pModel->GetMeshResource(iMeshIndex, eType, iTextureIndex, ppOut);
}

_uInt CVIBuffer_Model_Instance::GetModelIndex()
{
	return m_pModel->GetNumMeshes();
}

void CVIBuffer_Model_Instance::Bind_Resource(_uInt iMeshIndex)
{
	ID3D11Buffer* VertexBuffers[] = {
			m_pModel->GetMeshVertexBuffer(iMeshIndex, &m_iVertexStride),
		    m_pVBInstance,
	};

	_uInt		VertexStrides[] = {
	m_iVertexStride,
	m_iInstanceStride,
	};

	ID3D11Buffer* IndexBuffer = m_pModel->GetMeshIndexBuffer(iMeshIndex, &m_eIndexFormat, &m_iNumIndices);
	_uInt		Offsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, VertexBuffers, VertexStrides, Offsets);
	m_pContext->IASetIndexBuffer(IndexBuffer, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitive);
}

CVIBuffer_Model_Instance* CVIBuffer_Model_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pInstanceDesc)
{
	CVIBuffer_Model_Instance* pVIBuffer_Model = new CVIBuffer_Model_Instance(pDevice, pContext);
	if (FAILED(pVIBuffer_Model->Initialize_Prototype(pInstanceDesc)))
	{
		Safe_Release(pVIBuffer_Model);
		MSG_BOX("CREATE FAIL : VIBUFFER MODEL INSTANCE");
	}
	return pVIBuffer_Model;
}

CComponent* CVIBuffer_Model_Instance::Clone(void* pArg)
{
	CVIBuffer_Model_Instance* pVIBuffer_Model = new CVIBuffer_Model_Instance(*this);
	if (FAILED(pVIBuffer_Model->Initialize(pArg)))
	{
		Safe_Release(pVIBuffer_Model);
		MSG_BOX("CREATE FAIL : VIBUFFER MODEL INSTANCE");
	}
	return pVIBuffer_Model;
}

void CVIBuffer_Model_Instance::Free()
{
	__super::Free();

	Safe_Release(m_pModel);
	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pInstanceVertices);
	}
}
