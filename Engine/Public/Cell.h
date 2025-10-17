#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CVIBuffer_Cell;


typedef struct PortalDesc
{
	_float3		vLeftPoint,	vRightPoint;

	_Int		iCellAIndex;
	_Int		iCellBIndex;
}PORTAL_DESC;

class CCell : public CBase
{
public :
	enum class CELL_TYPE { GROUND, BUILD, WATER, END};

private :
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public :
	HRESULT					Initialize(_uInt iCellIndex, _uInt iCellProperity, const _float3* vPoints);

	_bool					IsCellIn(_vector vPos, _Int* pNeighborIndex);
	_bool					Compare(_vector vSourPoint, _vector vDestPoint);

	_vector					GetPoint(NAVI_POINT ePoint) { return XMLoadFloat3(&m_vTirPoints[ENUM_CLASS(ePoint)]); }
	_uInt					GetCellIndex() { return m_iCellIndex; }

	void					SetNeighbor(NAVI_LINE eLine, _Int iIndex);
	_bool					IsMoveAble();
	_float					ComputeHeight(_vector vPos);

	_vector					GetCellPoint(NAVI_POINT eType);
	_vector					GetCellCenterPoint();
	void					GetNeighborIndex(_Int* ppNeighborIndex);

	_vector					ComputeNormal();


	const PORTAL_DESC*		GetPortal(_Int iNeighborIndex);
#ifdef _DEBUG
	void					Render();
	void					Export(void* pArg);
#endif // _DEBUG

private :
#ifdef _DEBUG	
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	CVIBuffer_Cell*			m_pVIBuffer = { nullptr };
#endif

	_uInt					m_iCellIndex = {};
	_uInt					m_iCellProperity = {};

	_float3					m_fCenter = {};
	_float3					m_vTirPoints[ENUM_CLASS(NAVI_POINT::END)] = {};
	_float3					m_vTirNormals[ENUM_CLASS(NAVI_LINE::END)] = {};

	PORTAL_DESC				m_Portals[ENUM_CLASS(NAVI_LINE::END)] = {};
	_Int					m_NeighborIndices[ENUM_CLASS(NAVI_LINE::END)] = { -1, -1, -1 };
	_float4					m_vPlane = {};

	CELL_TYPE				m_eType;

public :
	static		CCell*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt iCellIndex, _uInt iCellProperity, const _float3* vPoints);
	virtual		void		Free() override;

};
NS_END