#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CMesh;
class CMaterial;
class CBone;
class CAnimation;

class ENGINE_DLL CModel : public CComponent
{
private :
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public :
	virtual HRESULT				Initialize_Prototype(MODEL_TYPE eType, const _char* pModelFilePath, _matrix PreModelMat = XMMatrixIdentity());
	virtual HRESULT				Initialize(void* pArg) override;

	virtual HRESULT				Render(_uInt iMeshIndex);

	HRESULT						GetMeshResource(_uInt iMeshIndex, aiTextureType eType, _uInt iTextureIndex, ID3D11ShaderResourceView** ppOut);
	_uInt						GetMeshNumBones(_uInt iMeshIndex) const;
	_uInt						GetNumMeshes() const { return m_iNumMeshes; }
	
	// Model의 뼈에대한 함수
	_Int						GetBoneIndex(const char* szBoneName) const;
	_uInt						GetNumBones() const  { return m_iNumBones; }
	_float4x4*					GetBoneMatrices(_uInt iMeshIndex);
	
	void						PlayAnimation(_uInt iCurrentAnimIndex, _float DeletaTime);

	void						Export(const char* FilePath);

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

	// 애니메이션 개수 및 애니메이션 저장
	_Int						m_iCurrentAnimIndex = { -1 };
	_uInt						m_iNumAnimations = {};
	vector<CAnimation*>			m_Animations;

private:
	/* fbx 파일의 정보를 가져오기위한 함수 */
	HRESULT						Ready_Meshes(_matrix PreModelMat = XMMatrixIdentity());
	HRESULT						Ready_Materials(const _char* pModelFilePath);
	HRESULT						Ready_Bones(const aiNode* pAINode, _Int iParentIndex);
	HRESULT						Ready_Animations();

	/* 인게임내에 만들어진 파일의 정보를 가져오기위한 함수 */
	HRESULT						Ready_Meshes(void* MeshDesc, _matrix PreModelMat = XMMatrixIdentity());
	HRESULT						Ready_Materials(void* MatrialDesc, const _char* pModelFilePath);
	HRESULT						Ready_Bones(void* BoneDesc);
	HRESULT						Ready_Animations(void* AnimationDesc);

	/* fbx 파일 저장하는 함수 */
	HRESULT						SaveModelFile(void* Data, const char* FilePath);
	HRESULT						SaveAnimModelFile(void* Data, const char* FilePath);

	/* 저장된 파일 읽어들이는 함수 */
	HRESULT						ReadModelFile(void* Data, const char* FilePath);

	void						ExportNonAnim(void *pArg);
	void						ExportAnim(void* pArg);

public:
	static		CModel*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, const _char* pModelFilePath, _matrix PreModelMat = XMMatrixIdentity());
	virtual		CComponent*		Clone(void* pArg);
	virtual		void			Free(); public:
};
NS_END