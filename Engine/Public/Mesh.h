#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)
class CModel; 
class CBone;

typedef struct Navi_Edge
{
	_float3 A;
	_float3 B;

	_bool operator== (const NAVI_EDGE& rhs)
	{
		if (XMVector3Equal(XMLoadFloat3(&A), XMLoadFloat3(&rhs.A)) &&
			XMVector3Equal(XMLoadFloat3(&B), XMLoadFloat3(&rhs.B)))
			return true;
	}

}NAVI_EDGE;

typedef struct Navi_Triangle
{
	_float3 A;
	_float3 B;
	_float3 C;

	list<NAVI_EDGE> TriEdge;

	_bool operator== (const NAVI_TRIANGLE& rhs)
	{
		if (XMVector3Equal(XMLoadFloat3(&A), XMLoadFloat3(&rhs.A)) &&
			XMVector3Equal(XMLoadFloat3(&B), XMLoadFloat3(&rhs.B)) &&
			XMVector3Equal(XMLoadFloat3(&C), XMLoadFloat3(&rhs.C)))
			return true;
	}
}NAVI_TRIANGLE;

class ENGINE_DLL CMesh : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	virtual HRESULT			Initialize_Prototype(MODEL_TYPE eType, const CModel* pModel, const aiMesh* pAIMesh, _matrix PreModelMat = XMMatrixIdentity());
	virtual HRESULT			Initialize_Prototype(MODEL_TYPE eType, void* MeshDesc, _matrix PreModelMat = XMMatrixIdentity());
	virtual HRESULT			Initialize(void* pArg) override;

	const _uInt				GetMatrialIndex() { return m_iMateriaIndex; }
	_uInt					GetMeshNumBone() const { return m_iNumBones; }
	_float4x4*				GetMeshBoneMatrices(const vector<CBone*>& Bones);

	HRESULT					Export(void* pOut);
	HRESULT					AnimExport(void* pOut);

	const _float3*			GetVerticesPoint() const { return m_pVertices; }
	_uInt					GetNumVertices() const { return m_iNumVertices; }
	void					GetIndices(vector<_uInt>& Indices);

private :
	// 메시의 이름을 보관해둔다.
	_char					m_szMeshName[MAX_PATH] = {};
	_uInt					m_iNumBones = {};

	// 메시에서 참조하고 사용할 뼈의 번호를 소유
	vector<_uInt>			m_BoneIndices = { };

	//셰이더에 넘기기 위한 뼈의 행렬정보
	_float4x4*				m_pBoneMatrices = { nullptr };

	// 같은뼈를 사용하는 메쉬의 경우에 사용할 행렬정보
	// 메쉬에 맞춰서 뼈의 크기및 위치를 조정하려는 형태이다.
	vector<_float4x4>		m_OffsetMatrices = {};
	
	_uInt					m_iMateriaIndex = {};

private :
	HRESULT					Ready_VertexBuffer_For_NonAnim(const aiMesh* pAIMesh, _matrix PreTransformMatrix);
	HRESULT					Ready_VertexBuffer_For_Anim(const CModel* pModel, const aiMesh* pAIMesh);

	HRESULT					Ready_VertexBuffer_For_NonAnim(void* MeshDesc, _matrix PreTransformMatrix);
	HRESULT					Ready_VertexBuffer_For_Anim(void* MeshDesc);

	//HRESULT				Ready_VertexBuffer_For_NonAnim(void* MeshDesc, _matrix PreTransformMatrix);
	//HRESULT				Ready_VertexBuffer_For_Anim(void* MeshDesc, const aiMesh* pAIMesh);


public:
	static CMesh*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, const CModel* pModel, const aiMesh* pAIMesh, _matrix PreModelMat = XMMatrixIdentity());
	static CMesh*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, void* MeshDesc, _matrix PreModelMat = XMMatrixIdentity());

	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};
NS_END