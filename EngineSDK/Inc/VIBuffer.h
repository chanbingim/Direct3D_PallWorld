#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CVIBuffer abstract : public CComponent
{

protected :
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public :
	virtual HRESULT			Initialize_Prototype(D3D11_BUFFER_DESC* vtxDesc, D3D11_BUFFER_DESC* idxDesc);
	virtual HRESULT			Initialize(void* pArg) override;

	virtual void			Render_VIBuffer();
	virtual _bool			IsPicking(class CTransform* pTransform, _float3* pOut);
	virtual _bool			IsPicking(CTransform* pTransform, _float3& vOut, _float3& vNormal);
	virtual _bool			IsPicking(_vector vRayOrizin, _vector vRayDir, class CTransform* pTransform, _float3* pOut);

	_uInt					GetNumVertices() const { return m_iNumIndices; }
	_uInt					GetNumIndices() const { return m_iNumIndices; }

protected :
	//¿Ã¡¶ D11Buffer∑Œ ≈Î«’µ  ¿Œµ¶Ω∫∂˚ πˆ≈√Ω∫∂˚
	_uInt						m_iNumVertices = {};
	_uInt						m_iVertexStride = {};
	_float3*					m_pVertices = nullptr;
	ID3D11Buffer*				m_pVertexBuffer = nullptr;

	_uInt						m_iNumIndices = {};
	_uInt						m_iIndexStride = {};
	ID3D11Buffer*				m_pIndexBuffer = nullptr;

	_uInt						m_iNumVertexBuffers = {};
	DXGI_FORMAT					m_eIndexFormat = {};
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitive = {};

public :
	virtual		CComponent*		Clone(void* pArg) override;
	virtual		void			Free() override;
};
NS_END