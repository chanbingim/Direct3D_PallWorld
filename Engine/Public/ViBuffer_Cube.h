#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Cube : public CVIBuffer
{
protected:
	CVIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Cube(const CVIBuffer_Cube& rhs);
	virtual ~CVIBuffer_Cube() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg) override;

	virtual void			Render_VIBuffer();

	_uInt					GetNumVertices() const { return m_iNumIndices; }
	_uInt					GetNumIndices() const { return m_iNumIndices; }

public:
	static		CVIBuffer_Cube*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*			Clone(void* pArg) override;
	virtual		void				Free() override;

};
NS_END