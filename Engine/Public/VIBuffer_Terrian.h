#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)
class CTransform;

class ENGINE_DLL CVIBuffer_Terrian final : public CVIBuffer
{
private:
	CVIBuffer_Terrian(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrian(const CVIBuffer_Terrian& rhs);
	virtual ~CVIBuffer_Terrian() = default;

public:
	HRESULT					Initialize_Prototype(_uInt TileCnt);
	HRESULT					Initialize_Prototype(const WCHAR* pHegithFilePath);
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Render_VIBuffer() override;

	HRESULT					ExportHeightMap(const WCHAR* ExportFilePath);
	
	virtual _bool			IsPicking(CTransform* pTransform, _float3* pOut) override;
	HRESULT					UpdateHegiht(CTransform* pTransform, _float Apply, _float Offset);

private :
	_float2					m_iNumVertex = {};

private :
	_bool					ComputeBoundingBox(CTransform* pTransform);



public:
	static		CVIBuffer_Terrian*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const WCHAR* pHegithFilePath);
	static		CVIBuffer_Terrian*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt TileCnt);

	virtual		CComponent*			Clone(void* pArg) override;
	virtual		void				Free() override;
};
NS_END