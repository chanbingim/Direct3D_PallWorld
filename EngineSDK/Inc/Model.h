#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CMesh;
class CMaterial;
class CBone;
class CTransform;
class CAnimation;

class ENGINE_DLL CModel : public CComponent
{
private :
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public :
	virtual HRESULT				Initialize_Prototype(MODEL_TYPE eType, const _char* pModelFilePath, _matrix PreModelMat = XMMatrixIdentity(), const char* RetargetFile = "", _uInt iLayerCount = 1);
	virtual HRESULT				Initialize(void* pArg, const char* RetargetFile = "");

	virtual HRESULT				Render(_uInt iMeshIndex);
	const _float3*				GetVerticesPoint(_uInt iMeshIndex) const;
	_uInt						GetNumVertices(_uInt iMeshIndex) const;
	void						GetIndices(_uInt iMeshIndex, vector<_uInt>& Indices) const;

	HRESULT						GetMeshResource(_uInt iMeshIndex, _uInt eType, _uInt iTextureIndex, ID3D11ShaderResourceView** ppOut);
	_uInt						GetMeshNumBones(_uInt iMeshIndex) const;
	_uInt						GetNumMeshes() const { return m_iNumMeshes; }
	_matrix						GetBoneTransformation(const char* szBoneName);
	_matrix						GetBoneTransformation(_uInt iIndex);

	// Model의 뼈에대한 함수
	_Int						GetBoneIndex(const char* szBoneName) const;
	_uInt						GetNumBones() const  { return m_iNumBones; }
	_float4x4*					GetTransformationOffsetMatrixPtr(const char* szBoneName);

	_float4x4*					GetBoneMatrices(_uInt iMeshIndex);
	const _float4x4*			GetCombinedTransformationMatrixPtr(const char* szBoneName) const;

	//모델 애니메이션 관련 함수
	_uInt						GetNumAnimations() { return m_iNumAnimations; }
	_bool						PlayAnimation(_uInt iAnimLayerIndex, _uInt iCurrentAnimIndex, _float fDeletaTime, _float fAnimSpeed = 10.f, _bool bIsLoop = true, const char* BoneName = "Root Node", const char* EndBoneName = "");
	_bool						ResetAnimation(_uInt iAnimLayerIndex);
	void						BindParentAnim(CModel* DstData);
	void						Bind_KeyFrameFunction(const char* szName, _uInt iKeyFrame, function<void()> function);

	const char*					GetAnimationName(_uInt iIndex);
	_Int						GetNumAnimation(const char* szName);
	void						Export(const char* FilePath);

	void						ExportMappingData(CModel* DstData, unordered_map<_string, pair<_Int, _Int>>* pOut);

	_bool						IsPicking(CTransform* pTransform, _float3* pOut, _uInt* OutiNumIndex);
	_bool						IsPicking(CTransform* pTransform, _float3& vOut, _float3& vOutNormal);

	_bool						IsPicking(_vector vRayOrizin, _vector vRayDir, CTransform* pTransform, _float3* pOut);

	ID3D11Buffer*				GetMeshVertexBuffer(_uInt iMeshIndex, _uInt* pOutVertexStride);
	ID3D11Buffer*				GetMeshIndexBuffer(_uInt iMeshIndex, DXGI_FORMAT* eFormat, _uInt* pIndices);
private :
	// Assimp Lib
#ifdef _DEBUG
	const aiScene*				m_pAIScene = { nullptr };
	Assimp::Importer			m_Importer;
#endif // _DEBUG

	_float4x4					m_PreTransformMatrix = {};
	MODEL_TYPE					m_eType = { MODEL_TYPE::END };

	// 매시 개수 및 매시 저장
	_uInt						m_iNumMeshes = {};
	vector<CMesh*>				m_Meshes;

	// 머테리얼 개수 및 머테리얼 저장
	_uInt						m_iNumMaterials = {};
	vector<CMaterial*>			m_Materials;

	// 뼈 개수 및 뼈 저장
	_uInt						m_iNumBones = {};
	vector<CBone*>				m_Bones;
	vector<_bool>				m_PreAnimBones, m_CurAnimBones;

	// 애니메이션 개수 및 애니메이션 저장
	_bool						m_bIsLerpAnimation = false;
	vector<_Int>				m_CurrentAnimIndexs = { -1 };
	_float2						m_PreFrameTrackPos;

	_uInt						m_iNumAnimations = {};
	vector<CAnimation*>			m_Animations;

	vector<_Int>				m_RetargetIndices;

private:
#ifdef _DEBUG
	/* fbx 파일의 정보를 가져오기위한 함수 */
	HRESULT						Ready_Meshes(_matrix PreModelMat = XMMatrixIdentity());
	HRESULT						Ready_Materials(const _char* pModelFilePath);
	HRESULT						Ready_Bones(const aiNode* pAINode, _Int iParentIndex);
	HRESULT						Ready_Animations();

	/* fbx 파일 저장하는 함수 */
	HRESULT						SaveModelFile(void* Data, const char* FilePath);
	HRESULT						SaveAnimModelFile(void* Data, const char* FilePath);

	void						ExportNonAnim(void* pArg);
	void						ExportAnim(void* pArg);
#endif

	/* 인게임내에 만들어진 파일의 정보를 가져오기위한 함수 */
	HRESULT						Ready_Meshes(void* MeshDesc, _matrix PreModelMat = XMMatrixIdentity());
	HRESULT						Ready_Materials(void* MatrialDesc, const _char* pModelFilePath);
	HRESULT						Ready_Bones(void* BoneDesc);
	HRESULT						Ready_Animations(void* AnimationDesc);

	/* 저장된 파일 읽어들이는 함수 */
	HRESULT						ReadModelFile(void* Data, const char* FilePath);

	/* 저장된 파일 리타겟팅 함수 */
	HRESULT						ReadReTargetlFile(const char* FilePath);
	HRESULT						ReadAnimModelFile(void* Data, const char* FilePath);

	void						ChangeAnimation(_uInt iAnimLayerIndex, _uInt iAnimIndex);
	_bool						LerpAnimation(_uInt iAnimLayerIndex, _float fDeletaTime, _float fAnimSpeed, _int2 UpdateBoneIdx);

public:
	static		CModel*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, const _char* pModelFilePath, _matrix PreModelMat = XMMatrixIdentity(), const char* RetargetFile = "", _uInt iLayerCount = 1);
	virtual		CComponent*		Clone(void* pArg);
	virtual		void			Free();
};
NS_END