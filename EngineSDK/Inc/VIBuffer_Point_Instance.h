#pragma once

#include "VIBuffer_Instance.h"

NS_BEGIN(Engine)
class CVIBuffer_Point_Instance : public CVIBuffer_Instance
{
protected:
	CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& Prototype);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT								Initialize_Prototype(const INSTANCE_DESC* pInstanceDesc) override;
	virtual HRESULT								Initialize(void* pArg) override;

	virtual void								Render_VIBuffer() override;
	VTX_INSTANCE_DEFAULT_DESC*					GetInstanceData() { return m_pInstanceVertices; }
	
private :
	VTX_INSTANCE_DEFAULT_DESC*					m_pInstanceVertices = { nullptr };

protected:
	virtual		void							Bind_Resource() override;

public:
	static		CVIBuffer_Point_Instance*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pInstanceDesc);
	virtual		CComponent*						Clone(void* pArg) override;
	virtual		void							Free() override;
};
NS_END
