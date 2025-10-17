#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Instance abstract : public CVIBuffer
{
public :
	typedef struct tagInstanceDesc
	{
		_uInt		iNumInstance = {};
		_float3		vSize = {};
		_float3		vCenter = {};
		_float3		vRange = {};
	}INSTANCE_DESC;

protected:
	CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instance(const CVIBuffer_Instance& Prototype);
	virtual ~CVIBuffer_Instance() = default;

public:
	virtual HRESULT		Initialize_Prototype(const INSTANCE_DESC* pInstanceDesc);
	virtual HRESULT		Initialize(void* pArg) override;

	virtual void		Render_VIBuffer() override;

	_uInt				GetNumInstance() { return m_iNumInstance; }
	void				Lock(D3D11_MAPPED_SUBRESOURCE* pSubResource);
	void				UnLock();

	_float3				GetInstanceModelBoundSize();

protected:
	ID3D11Buffer*			m_pVBInstance = { nullptr };
	D3D11_BUFFER_DESC		m_InstanceBufferDesc = {};
	D3D11_SUBRESOURCE_DATA	m_InstanceInitialDesc = {};

	_float3					m_vBoxSize = {};
	_uInt					m_iInstanceStride = {};
	_uInt					m_iNumInstance = {};
	_uInt					m_iNumIndexPerInstance = {};

protected :
	virtual		void		Bind_Resource();

public:
	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};
NS_END
