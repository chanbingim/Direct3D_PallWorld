#include "Mesh.h"

#include "PaseDataHeader.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs) :
    CVIBuffer(rhs)
{
}

HRESULT CMesh::Initialize_Prototype(MODEL_TYPE eType, const aiMesh* pAIMesh, _matrix PreModelMat)
{
	m_iNumVertexBuffers = 1;
	//정점의 수를 꺼내와서 저장
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iMateriaIndex = pAIMesh->mMaterialIndex;

	m_iVertexStride = sizeof(VTX_MESH);

	//삼각형의 개수를 꺼내와서 저장
	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	m_pVertices = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);

	D3D11_SUBRESOURCE_DATA	InitialVBData{};

	VTX_MESH* pVtxMeshs = new VTX_MESH[m_iNumVertices];
	ZeroMemory(pVtxMeshs, sizeof(VTX_MESH) * m_iNumVertices);

	for (_uInt i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVtxMeshs[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVtxMeshs[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVtxMeshs[i].vPosition), PreModelMat));
		
		memcpy(&pVtxMeshs[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVtxMeshs[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVtxMeshs[i].vNormal), PreModelMat));

		memcpy(&pVtxMeshs[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVtxMeshs[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}
	InitialVBData.pSysMem = pVtxMeshs;
	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVertexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVtxMeshs);
	
#pragma endregion

#pragma region INDEX_BUFFER
	D3D11_BUFFER_DESC		IBDesc{};
	IBDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.StructureByteStride = m_iIndexStride;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	_uInt* pIndices = new _uInt[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uInt) * m_iNumIndices);

	_uInt	iNumIndices = {};
	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[0];
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[1];
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[2];
	}

	D3D11_SUBRESOURCE_DATA	InitialIBData{};
	InitialIBData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitialIBData, &m_pIndexBuffer)))
		return E_FAIL;
#pragma endregion

	Safe_Delete_Array(pIndices);

    return S_OK;
}

HRESULT CMesh::Initialize_Prototype(MODEL_TYPE eType,  void* MeshDesc, _matrix PreModelMat)
{
	if (nullptr == MeshDesc)
		return E_FAIL;

	SAVE_MESH_DESC* pMeshDesc = static_cast<SAVE_MESH_DESC*>(MeshDesc);
	m_iNumVertexBuffers = 1;
	//정점의 수를 꺼내와서 저장
	m_iNumVertices = pMeshDesc->iNumVertices;
	m_iMateriaIndex = pMeshDesc->iNumMaterialIndex;

	m_iVertexStride = sizeof(VTX_MESH);

	//삼각형의 개수를 꺼내와서 저장
	m_iNumIndices = pMeshDesc->iNumFaces * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTX_MESH* pVtxMeshs = new VTX_MESH[m_iNumVertices];
	ZeroMemory(pVtxMeshs, sizeof(VTX_MESH) * m_iNumVertices);

	m_pVertices = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);

	_uInt iIndex = {};
	for (auto& vertex : pMeshDesc->Vertices)
	{
		memcpy(&pVtxMeshs[iIndex].vPosition, &vertex.vPosition, sizeof(_float3));
		XMStoreFloat3(&pVtxMeshs[iIndex].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVtxMeshs[iIndex].vPosition), PreModelMat));

		memcpy(&pVtxMeshs[iIndex].vNormal, &vertex.vNormal, sizeof(_float3));
		XMStoreFloat3(&pVtxMeshs[iIndex].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVtxMeshs[iIndex].vNormal), PreModelMat));

		memcpy(&pVtxMeshs[iIndex].vTangent, &vertex.vTangent, sizeof(_float3));
		memcpy(&pVtxMeshs[iIndex].vTexcoord, &vertex.vTexcoord, sizeof(_float2));
		iIndex++;
	}

	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	InitialVBData.pSysMem = pVtxMeshs;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVertexBuffer)))
		return E_FAIL;
#pragma endregion

#pragma region INDEX_BUFFER
	D3D11_BUFFER_DESC		IBDesc{};
	IBDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.StructureByteStride = m_iIndexStride;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	_uInt* pIndices = new _uInt[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uInt) * m_iNumIndices);

	_uInt iNumIndices = {};
	for (auto& Index : pMeshDesc->Indices)
		pIndices[iNumIndices++] = Index;

	D3D11_SUBRESOURCE_DATA	InitialIBData{};
	InitialIBData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitialIBData, &m_pIndexBuffer)))
		return E_FAIL;
#pragma endregion
	Safe_Delete_Array(pVtxMeshs);
	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CMesh::Export(void* pOut)
{
	if (nullptr == pOut)
		return E_FAIL;

	SAVE_MESH_DESC* MeshDesc = static_cast<SAVE_MESH_DESC*>(pOut);
	MeshDesc->iNumVertices = m_iNumVertices;
	MeshDesc->iNumFaces = m_iNumIndices / 3;
	MeshDesc->iNumMaterialIndex = m_iMateriaIndex;

	ID3D11Buffer* StagingBuffer = nullptr;

	//버퍼 세팅
	D3D11_BUFFER_DESC			VertexDesc = {};
	VertexDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VertexDesc.Usage = D3D11_USAGE_STAGING;
	VertexDesc.BindFlags = 0;
	VertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	VertexDesc.MiscFlags = 0;
	VertexDesc.StructureByteStride = m_iVertexStride;
	m_pDevice->CreateBuffer(&VertexDesc, nullptr, &StagingBuffer);

	if (StagingBuffer)
	{
		m_pContext->CopyResource(StagingBuffer, m_pVertexBuffer);

		D3D11_MAPPED_SUBRESOURCE VerticesMap;
		if (S_OK == m_pContext->Map(StagingBuffer, 0, D3D11_MAP_READ, 0, &VerticesMap))
		{
			VTX_MESH* pVertices = reinterpret_cast<VTX_MESH*>(VerticesMap.pData);
			for (_uInt i = 0; i < m_iNumVertices; ++i)
				MeshDesc->Vertices.push_back(pVertices[i]);

			m_pContext->Unmap(StagingBuffer, 0);
		}
	}
	Safe_Release(StagingBuffer);

	//버퍼 세팅
	D3D11_BUFFER_DESC			IndexDesc = {};
	IndexDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	IndexDesc.Usage = D3D11_USAGE_STAGING;
	IndexDesc.BindFlags = 0;
	IndexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	IndexDesc.MiscFlags = 0;
	IndexDesc.StructureByteStride = m_iIndexStride;
	m_pDevice->CreateBuffer(&IndexDesc, nullptr, &StagingBuffer);

	if (StagingBuffer)
	{
		m_pContext->CopyResource(StagingBuffer, m_pIndexBuffer);

		D3D11_MAPPED_SUBRESOURCE IndicesMap;
		if (S_OK == m_pContext->Map(StagingBuffer, 0, D3D11_MAP_READ, 0, &IndicesMap))
		{
			_uInt* pIndices = reinterpret_cast<_uInt*>(IndicesMap.pData);
			for (_uInt i = 0; i < m_iNumIndices; ++i)
				MeshDesc->Indices.push_back(pIndices[i]);

			m_pContext->Unmap(StagingBuffer, 0);
		}
	}
	Safe_Release(StagingBuffer);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, const aiMesh* pAIMesh, _matrix PreModelMat)
{
	CMesh* pMesh = new CMesh(pDevice, pContext);
	if (FAILED(pMesh->Initialize_Prototype(eType, pAIMesh, PreModelMat)))
	{
		Safe_Release(pMesh);
		MSG_BOX("CREATE FAIL : MESH");
	}

	return pMesh;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, void* MeshDesc, _matrix PreModelMat)
{
	CMesh* pMesh = new CMesh(pDevice, pContext);
	if (FAILED(pMesh->Initialize_Prototype(eType, MeshDesc, PreModelMat)))
	{
		Safe_Release(pMesh);
		MSG_BOX("CREATE FAIL : MESH");
	}

	return pMesh;
}

CComponent* CMesh::Clone(void* pArg)
{
	CMesh* pMesh = new CMesh(*this);
	if (FAILED(pMesh->Initialize(pArg)))
	{
		Safe_Release(pMesh);
		MSG_BOX("CLONE FAIL : MESH");
	}

	return pMesh;
}

void CMesh::Free()
{
    __super::Free();
}
