#include "Mesh.h"

#include "GameInstance.h"
#include "PaseDataHeader.h"
#include "Model.h"
#include "Bone.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs) :
    CVIBuffer(rhs)
{
}

HRESULT CMesh::Initialize_Prototype(MODEL_TYPE eType, const CModel* pModel, const aiMesh* pAIMesh, _matrix PreModelMat)
{
	strcpy_s(m_szMeshName, pAIMesh->mName.data);
	m_iNumVertexBuffers = 1;
	//정점의 수를 꺼내와서 저장
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iMateriaIndex = pAIMesh->mMaterialIndex;
	

	//삼각형의 개수를 꺼내와서 저장
	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	MODEL_TYPE::NONANIM == eType ? Ready_VertexBuffer_For_NonAnim(pAIMesh, PreModelMat) :
								   Ready_VertexBuffer_For_Anim(pModel, pAIMesh);
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

	MODEL_TYPE::NONANIM == eType ? Ready_VertexBuffer_For_NonAnim(MeshDesc, PreModelMat) :
								   Ready_VertexBuffer_For_Anim(MeshDesc);

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
	vector<_uInt>& Indices = MODEL_TYPE::NONANIM == eType ?  static_cast<SAVE_MESH_DESC*>(MeshDesc)->Indices :
															 static_cast<SAVE_ANIM_MESH_DESC*>(MeshDesc)->Indices;
	for (auto& Index : Indices)
		pIndices[iNumIndices++] = Index;

	D3D11_SUBRESOURCE_DATA	InitialIBData{};
	InitialIBData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &InitialIBData, &m_pIndexBuffer)))
		return E_FAIL;
#pragma endregion
	
	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
    return S_OK;
}

_float4x4* CMesh::GetMeshBoneMatrices(const vector<CBone*>& Bones)
{
	for (size_t i = 0; i < m_iNumBones; i++)
	{
		/* 최종적으로 렌더링하기위한 뼈의 행렬(CombinedTransformationMatrix). */
		XMStoreFloat4x4(&m_pBoneMatrices[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * Bones[m_BoneIndices[i]]->GetCombinedTransformationMatrix());
	}
	return m_pBoneMatrices;
}

HRESULT CMesh::Export(void* pOut)
{
	if (nullptr == pOut)
		return E_FAIL;

	SAVE_MESH_DESC* MeshDesc = static_cast<SAVE_MESH_DESC*>(pOut);
	strcpy_s(MeshDesc->szName, m_szMeshName);
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

HRESULT CMesh::AnimExport(void* pOut)
{
	if (nullptr == pOut)
		return E_FAIL;

	SAVE_ANIM_MESH_DESC* MeshDesc = static_cast<SAVE_ANIM_MESH_DESC*>(pOut);
	strcpy_s(MeshDesc->szName, m_szMeshName);
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
			VTX_ANIM_MESH* pVertices = reinterpret_cast<VTX_ANIM_MESH*>(VerticesMap.pData);
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

	MeshDesc->iNumBones = m_iNumBones;
	MeshDesc->BoneIndices = m_BoneIndices;
	MeshDesc->OffsetMatrices = m_OffsetMatrices;
	return S_OK;
}

void CMesh::GetIndices(vector<_uInt>& Indices)
{
	ID3D11Buffer* StagingBuffer = nullptr;
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
				Indices.push_back(pIndices[i]);

			m_pContext->Unmap(StagingBuffer, 0);
		}
	}
	Safe_Release(StagingBuffer);
}

_bool CMesh::IsPicking(CTransform* pTransform, _float3* pOut)
{
	_matrix InvWorld = XMLoadFloat4x4(&pTransform->GetInvWorldMat());
	m_pGameInstance->Compute_LocalRay(&InvWorld);

	ID3D11Buffer* StagingBuffer = nullptr;
	//버퍼 세팅
	D3D11_BUFFER_DESC			IndexDesc = {};
	IndexDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	IndexDesc.Usage = D3D11_USAGE_STAGING;
	IndexDesc.BindFlags = 0;
	IndexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	IndexDesc.MiscFlags = 0;
	IndexDesc.StructureByteStride = m_iIndexStride;
	m_pDevice->CreateBuffer(&IndexDesc, nullptr, &StagingBuffer);

	_bool bIsPicking = false;
	if (StagingBuffer)
	{
		m_pContext->CopyResource(StagingBuffer, m_pIndexBuffer);

		D3D11_MAPPED_SUBRESOURCE IndicesMap;
		if (S_OK == m_pContext->Map(StagingBuffer, 0, D3D11_MAP_READ, 0, &IndicesMap))
		{
			_uInt* pIndices = reinterpret_cast<_uInt*>(IndicesMap.pData);
			for (_uInt i = 0; i < m_iNumIndices; i += 3)
			{
				//하단 삼각형
				if (true == m_pGameInstance->Picking_InLocal(m_pVertices[pIndices[i]], m_pVertices[pIndices[i+ 1]], m_pVertices[pIndices[i+ 2]], pOut))
				{
					bIsPicking = true;
					XMStoreFloat3(pOut, XMVector3TransformCoord(XMLoadFloat3(pOut), XMLoadFloat4x4(&pTransform->GetWorldMat())));
					break;
				}
			}

			m_pContext->Unmap(StagingBuffer, 0);
		}
	}
	Safe_Release(StagingBuffer);

	return bIsPicking;
}

_bool CMesh::IsPicking(_vector vRayOrizin, _vector vRayDir, CTransform* pTransform, _float3* pOut)
{
	_matrix InvWorld = XMLoadFloat4x4(&pTransform->GetInvWorldMat());

	vRayOrizin = XMVector3TransformCoord(vRayOrizin, InvWorld);

	vRayDir = XMVector3TransformNormal(vRayDir, InvWorld);
	vRayDir = XMVector3Normalize(vRayDir);

	ID3D11Buffer* StagingBuffer = nullptr;
	//버퍼 세팅
	D3D11_BUFFER_DESC			IndexDesc = {};
	IndexDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	IndexDesc.Usage = D3D11_USAGE_STAGING;
	IndexDesc.BindFlags = 0;
	IndexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	IndexDesc.MiscFlags = 0;
	IndexDesc.StructureByteStride = m_iIndexStride;
	m_pDevice->CreateBuffer(&IndexDesc, nullptr, &StagingBuffer);

	_bool bIsPicking = false;
	if (StagingBuffer)
	{
		m_pContext->CopyResource(StagingBuffer, m_pIndexBuffer);

		D3D11_MAPPED_SUBRESOURCE IndicesMap;
		if (S_OK == m_pContext->Map(StagingBuffer, 0, D3D11_MAP_READ, 0, &IndicesMap))
		{
			_uInt* pIndices = reinterpret_cast<_uInt*>(IndicesMap.pData);
			for (_uInt i = 0; i < m_iNumIndices;)
			{
				//하단 삼각형
				if (true == m_pGameInstance->RayPicking(vRayOrizin, vRayDir, m_pVertices[pIndices[i++]], m_pVertices[pIndices[i++]], m_pVertices[pIndices[i++]], pOut))
				{
					bIsPicking = true;
					XMStoreFloat3(pOut, XMVector3TransformCoord(XMLoadFloat3(pOut), XMLoadFloat4x4(&pTransform->GetWorldMat())));
					break;
				}
			}
			
			m_pContext->Unmap(StagingBuffer, 0);
		}
	}
	Safe_Release(StagingBuffer);

	return bIsPicking;
}

HRESULT CMesh::Ready_VertexBuffer_For_NonAnim(const aiMesh* pAIMesh, _matrix PreTransformMatrix)
{
	m_iVertexStride = sizeof(VTX_MESH);

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
		XMStoreFloat3(&pVtxMeshs[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVtxMeshs[i].vPosition), PreTransformMatrix));
		m_pVertices[i] = pVtxMeshs[i].vPosition;

		memcpy(&pVtxMeshs[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVtxMeshs[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVtxMeshs[i].vNormal), PreTransformMatrix));

		memcpy(&pVtxMeshs[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		XMStoreFloat3(&pVtxMeshs[i].vTangent, XMVector3TransformCoord(XMLoadFloat3(&pVtxMeshs[i].vTangent), PreTransformMatrix));

		memcpy(&pVtxMeshs[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}
	InitialVBData.pSysMem = pVtxMeshs;
	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVertexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVtxMeshs);
	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_Anim(const CModel* pModel, const aiMesh* pAIMesh)
{
	m_iVertexStride = sizeof(VTX_ANIM_MESH);
	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.StructureByteStride = m_iVertexStride;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	VTX_ANIM_MESH* pVtxAnimMeshs = new VTX_ANIM_MESH[m_iNumVertices];
	ZeroMemory(pVtxAnimMeshs, sizeof(VTX_ANIM_MESH) * m_iNumVertices);

	m_pVertices = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);

	for (_uInt i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVtxAnimMeshs[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVtxAnimMeshs[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVtxAnimMeshs[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVtxAnimMeshs[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	m_iNumBones = pAIMesh->mNumBones;
	m_OffsetMatrices.reserve(m_iNumBones);

	m_pBoneMatrices = new _float4x4[0 == m_iNumBones ? 1 : m_iNumBones];
	ZeroMemory(m_pBoneMatrices, sizeof(_float4x4) * m_iNumBones);

	_float4x4		OffsetMatrix;
	XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
	for (size_t i = 0; i < m_iNumBones; i++)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		// 오프셋 메트릭스는 매시안에 뼈정보가 가지고있다.
		// 그리고 Assimp로 행렬을 가져오게되면 Col Major행렬이라 전치로 바꿔야
		// Row Major 행렬로 바뀌게 된다.
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		// 뼈의 이름을 통해서 뼈의 이름과 일치하는 인덱스를 Model에서 얻어옴
		// 모델에 이용되는 모든뼈는 모델이 소유하고잇다.
		_Int		iBoneIndex = pModel->GetBoneIndex(pAIBone->mName.data);
		

		//부모가 없다면 웨이트 및 Index를 갱신할 이유가 없음
		if (-1 == iBoneIndex)
			return E_FAIL;

		m_OffsetMatrices.push_back(OffsetMatrix);
		m_BoneIndices.push_back(iBoneIndex);

		/* 이 뼈는 몇개의 정점에게 영향을 주는가? */
		for (size_t j = 0; j < pAIBone->mNumWeights; j++)
		{
			aiVertexWeight	AIWeight = pAIBone->mWeights[j];
			if (0.f == pVtxAnimMeshs[AIWeight.mVertexId].vBlendWeight.x)
			{
				pVtxAnimMeshs[AIWeight.mVertexId].vBlendIndex.x = (uint32_t)i;
				pVtxAnimMeshs[AIWeight.mVertexId].vBlendWeight.x = AIWeight.mWeight;
			}
			else if (0.f == pVtxAnimMeshs[AIWeight.mVertexId].vBlendWeight.y)
			{
				pVtxAnimMeshs[AIWeight.mVertexId].vBlendIndex.y = (uint32_t)i;
				pVtxAnimMeshs[AIWeight.mVertexId].vBlendWeight.y = AIWeight.mWeight;
			}
			else if (0.f == pVtxAnimMeshs[AIWeight.mVertexId].vBlendWeight.z)
			{
				pVtxAnimMeshs[AIWeight.mVertexId].vBlendIndex.z = (uint32_t)i;
				pVtxAnimMeshs[AIWeight.mVertexId].vBlendWeight.z = AIWeight.mWeight;
			}
			else
			{
				pVtxAnimMeshs[AIWeight.mVertexId].vBlendIndex.w = (uint32_t)i;
				pVtxAnimMeshs[AIWeight.mVertexId].vBlendWeight.w = AIWeight.mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;
		m_BoneIndices.push_back(pModel->GetBoneIndex(m_szMeshName));
		m_OffsetMatrices.push_back(OffsetMatrix);
	}

	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	InitialVBData.pSysMem = pVtxAnimMeshs;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVertexBuffer)))
		return E_FAIL;
	Safe_Delete_Array(pVtxAnimMeshs);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_NonAnim(void* MeshDesc, _matrix PreTransformMatrix)
{
	SAVE_MESH_DESC* pMeshDesc = static_cast<SAVE_MESH_DESC*>(MeshDesc);

	strcpy_s(m_szMeshName, pMeshDesc->szName);
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
		XMStoreFloat3(&pVtxMeshs[iIndex].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVtxMeshs[iIndex].vPosition), PreTransformMatrix));
		m_pVertices[iIndex] = pVtxMeshs[iIndex].vPosition;

		memcpy(&pVtxMeshs[iIndex].vNormal, &vertex.vNormal, sizeof(_float3));
		XMStoreFloat3(&pVtxMeshs[iIndex].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVtxMeshs[iIndex].vNormal), PreTransformMatrix));

		memcpy(&pVtxMeshs[iIndex].vTangent, &vertex.vTangent, sizeof(_float3));
		XMStoreFloat3(&pVtxMeshs[iIndex].vTangent, XMVector3TransformCoord(XMLoadFloat3(&pVtxMeshs[iIndex].vTangent), PreTransformMatrix));
		memcpy(&pVtxMeshs[iIndex].vTexcoord, &vertex.vTexcoord, sizeof(_float2));
		iIndex++;
	}

	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	InitialVBData.pSysMem = pVtxMeshs;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVertexBuffer)))
		return E_FAIL;
	Safe_Delete_Array(pVtxMeshs);
#pragma endregion
	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_Anim(void* MeshDesc)
{
	SAVE_ANIM_MESH_DESC* pMeshDesc = static_cast<SAVE_ANIM_MESH_DESC*>(MeshDesc);

	strcpy_s(m_szMeshName, pMeshDesc->szName);
	m_iNumVertexBuffers = 1;
	//정점의 수를 꺼내와서 저장
	m_iNumVertices = pMeshDesc->iNumVertices;
	m_iMateriaIndex = pMeshDesc->iNumMaterialIndex;

	m_iVertexStride = sizeof(VTX_ANIM_MESH);

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

	VTX_ANIM_MESH* pVtxMeshs = new VTX_ANIM_MESH[m_iNumVertices];
	ZeroMemory(pVtxMeshs, sizeof(VTX_ANIM_MESH) * m_iNumVertices);

	m_pVertices = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(_float3) * m_iNumVertices);

	_uInt iIndex = {};
	for (auto& vertex : pMeshDesc->Vertices)
	{
		memcpy(&pVtxMeshs[iIndex].vPosition, &vertex.vPosition, sizeof(_float3));
		memcpy(&pVtxMeshs[iIndex].vNormal, &vertex.vNormal, sizeof(_float3));
		memcpy(&pVtxMeshs[iIndex].vTangent, &vertex.vTangent, sizeof(_float3));
		memcpy(&pVtxMeshs[iIndex].vTexcoord, &vertex.vTexcoord, sizeof(_float2));
		memcpy(&pVtxMeshs[iIndex].vBlendIndex, &vertex.vBlendIndex, sizeof(XMUINT4));
		memcpy(&pVtxMeshs[iIndex].vBlendWeight, &vertex.vBlendWeight, sizeof(_float4));
		iIndex++;
	}

	D3D11_SUBRESOURCE_DATA	InitialVBData{};
	InitialVBData.pSysMem = pVtxMeshs;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &InitialVBData, &m_pVertexBuffer)))
		return E_FAIL;
	Safe_Delete_Array(pVtxMeshs);

	m_iNumBones = pMeshDesc->iNumBones;
	m_pBoneMatrices = new _float4x4[m_iNumBones];
	m_OffsetMatrices = pMeshDesc->OffsetMatrices;
	m_BoneIndices = pMeshDesc->BoneIndices;

#pragma endregion

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, const CModel* pModel, const aiMesh* pAIMesh, _matrix PreModelMat)
{
	CMesh* pMesh = new CMesh(pDevice, pContext);
	if (FAILED(pMesh->Initialize_Prototype(eType, pModel, pAIMesh, PreModelMat)))
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

	Safe_Delete_Array(m_pBoneMatrices);
}
