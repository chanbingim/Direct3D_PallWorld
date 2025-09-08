#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CCell : public CBase
{
private :
	CCell() = default;
	virtual ~CCell() = default;

public :
	HRESULT					Initialize(_uInt iCellIndex, _uInt iCellProperity, const _float3* vPoints);

private :
	_uInt					m_iCellIndex = {};
	_uInt					m_iCellProperity = {};
	_float3					m_vTirPoints[ENUM_CLASS(NAVI_POINT::END)] = {};
	_float3					m_vTirNormals[ENUM_CLASS(NAVI_LINE::END)] = {};

public :
	static		CCell*		Create(_uInt iCellIndex, _uInt iCellProperity, const _float3* vPoints);
	virtual		void		Free() override;

};
NS_END