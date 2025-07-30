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

protected :
	//¿Ã¡¶ D11Buffer∑Œ ≈Î«’µ  ¿Œµ¶Ω∫∂˚ πˆ≈√Ω∫∂˚
	_uInt						m_NumVertices = {};
	ID3D11Buffer*				m_pVertexBuffer = nullptr;

	_uInt						m_NumIndices = {};
	ID3D11Buffer*				m_pIndexBuffer = nullptr;

	VTX_TEX*					m_Vertices = nullptr;
	_uInt*						m_Indices = nullptr;

public :
	virtual		CComponent*		Clone(void* pArg) override;
	virtual		void			Free() override;
};
NS_END