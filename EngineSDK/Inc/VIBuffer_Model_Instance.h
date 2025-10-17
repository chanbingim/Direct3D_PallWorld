#pragma once
#include "VIBuffer_Instance.h"

NS_BEGIN(Engine)
class CShader;
class CModel;

class ENGINE_DLL CVIBuffer_Model_Instance : public CVIBuffer_Instance
{
public:
	typedef struct tagModelInstanceDesc : public INSTANCE_DESC
	{
		const _char*				pModelFilePath;
		_matrix						PreModelMat;
		const char*					RetargetFile = "";
		_uInt						iLayerCount = 1;
	}MODEL_INSTANCE_DESC;

protected:
	CVIBuffer_Model_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Model_Instance(const CVIBuffer_Model_Instance& Prototype);
	virtual ~CVIBuffer_Model_Instance() = default;

public:
	virtual HRESULT		Initialize_Prototype(const INSTANCE_DESC* pInstanceDesc);
	virtual HRESULT		Initialize(void* pArg) override;

	virtual void		Render_VIBuffer(_uInt iIndex);

	HRESULT				GetMeshResource(_uInt iMeshIndex, aiTextureType eType, _uInt iTextureIndex, ID3D11ShaderResourceView** ppOut);
	_uInt				GetModelIndex();

protected:
	CModel*						m_pModel = nullptr;
	VTX_INSTANCE_DEFAULT_DESC*	m_pInstanceVertices = nullptr;

protected:
	virtual		void	Bind_Resource(_uInt iMeshIndex);

public:
	static	CVIBuffer_Model_Instance*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pInstanceDesc);
	virtual CComponent*					Clone(void* pArg) override;
	virtual void						Free() override;
};
NS_END