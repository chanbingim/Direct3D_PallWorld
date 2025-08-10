#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CModel : public CComponent
{
private :
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public :
	virtual HRESULT				Initialize_Prototype(const _char* pModelFilePath);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual HRESULT				Render();

private :
	// Assimp Lib
	const aiScene*				m_pAIScene = { nullptr };
	Assimp::Importer			m_Importer;

	// 매시 개수 및 매시 저장
	_uInt						m_iNumMeshes = {};
	vector<class CMesh*>		m_Meshes;

private:
	HRESULT						Ready_Meshes();

public:
	static		CModel*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath);
	virtual		CComponent*		Clone(void* pArg);
	virtual		void			Free(); public:
};
NS_END