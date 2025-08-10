#include "Mesh.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs) :
    CVIBuffer(rhs)
{
}

HRESULT CMesh::Initialize_Prototype(const aiMesh* pAIMesh)
{
	m_iNumVertexBuffers = 1;
	//정점의 수를 꺼내와서 저장
	m_iNumVertices = pAIMesh->mNumVertices;
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

	VTX_MESH* pVtxMeshs = new VTX_MESH[m_iNumVertices];
	ZeroMemory(pVtxMeshs, sizeof(VTX_MESH) * m_iNumVertices);

	m_pVertices = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);

	for (_uInt i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVtxMeshs[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVtxMeshs[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVtxMeshs[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVtxMeshs[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
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
	Safe_Delete_Array(pVtxMeshs);
	Safe_Delete_Array(pIndices);

    return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
    return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh)
{
	CMesh* pMesh = new CMesh(pDevice, pContext);
	if (FAILED(pMesh->Initialize_Prototype(pAIMesh)))
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
