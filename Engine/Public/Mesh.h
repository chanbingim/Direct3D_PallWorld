#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)
class ENGINE_DLL CMesh : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	virtual HRESULT			Initialize_Prototype(MODEL_TYPE eType, const aiMesh* pAIMesh);
	virtual HRESULT			Initialize_Prototype(MODEL_TYPE eType, void* MeshDesc);
	virtual HRESULT			Initialize(void* pArg) override;

	const _uInt				GetMatrialIndex() { return m_iMateriaIndex; }

	HRESULT					Export(void* pOut);

private :
	_uInt					m_iMateriaIndex = {};

public:
	static CMesh*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, const aiMesh* pAIMesh);
	static CMesh*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, void* MeshDesc);

	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};
NS_END