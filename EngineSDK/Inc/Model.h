#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CMesh;
class CMaterial;

class ENGINE_DLL CModel : public CComponent
{
private :
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public :
	virtual HRESULT				Initialize_Prototype(const _char* pModelFilePath);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual HRESULT				Render(_uInt iMeshIndex);

	HRESULT						GetMeshResource(_uInt iMeshIndex, aiTextureType eType, _uInt iTextureIndex, ID3D11ShaderResourceView** ppOut);
	_uInt						GetNumMeshes() { return m_iNumMeshes; }

private :
	// Assimp Lib
	const aiScene*				m_pAIScene = { nullptr };
	Assimp::Importer			m_Importer;

	// 매시 개수 및 매시 저장
	_uInt						m_iNumMeshes = {};
	vector<class CMesh*>		m_Meshes;

	// 머테리얼 개수 및 머테리얼 저장
	_uInt						m_iNumMaterials = {};
	vector<class CMaterial*>	m_Materials;

private:
	HRESULT						Ready_Meshes();
	HRESULT						Ready_Materials(const _char* pModelFilePath);

public:
	static		CModel*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath);
	virtual		CComponent*		Clone(void* pArg);
	virtual		void			Free(); public:
};
NS_END