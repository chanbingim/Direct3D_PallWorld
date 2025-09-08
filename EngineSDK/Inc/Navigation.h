#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CNavigation : public CComponent
{
public:
	typedef struct tagNavigaionDesc
	{
		//현재 셀인덱스는 시작위치를 설정하여 그위치로 부터 탐색
		_Int				iCurrentCellIndex = { -1 };

	}NAVIGATION_DESC;

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT					Initialize_Prototype(const _tchar* pNavigationDataFiles);
	virtual HRESULT					Initialize(void* pArg);

private:
	_Int							m_iCurrentCellIndex = { -1 };
	vector<class CCell*>			m_Cells;

public:
	static CNavigation*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFiles);
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END