#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CVIBuffer_Terrain;
class CTransform;

class ENGINE_DLL CNavigation : public CComponent
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
	virtual HRESULT					Initialize_Prototype(const CVIBuffer_Terrain* pTerrianBuffer);
	virtual HRESULT					Initialize(void* pArg);

	void							Update(_matrix WorldMatrix);
	_bool							IsMove(_vector vPosition);

	void							ComputeHeight(CTransform* pTransform);
#ifdef _DEBUG
public:
	HRESULT							Export(const char* FilePath);
	HRESULT							Render(_float4 vColor, _bool DarwCurCell = false);
#endif

private:
	_Int							m_iCurrentCellIndex = { -1 };
	_float2							m_iNumNaviSize = {};

	static _float4x4				m_WorldMatrix;
	vector<class CCell*>			m_Cells;

#ifdef _DEBUG
private:
	class CShader*					m_pShaderCom = { nullptr };
	ID3DX11EffectMatrixVariable*	m_pEMVWorldMat = nullptr;
	ID3DX11EffectMatrixVariable*	m_pEMVViewMat = nullptr;
	ID3DX11EffectMatrixVariable*	m_pEMVProjMat = nullptr;
	ID3DX11EffectVariable*			m_pEVvColor = nullptr;

private :
	void							BindSahderResource();
	void							ApplyShaderReSource(_float4 vColor);
#endif

private:
	void							SetUpNeighbors();

public:
	static CNavigation*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFiles);
	static CNavigation*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CVIBuffer_Terrain* pTerrianBuffer);

	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END