#pragma once
#include "NavigationStruct.h"
#include "Component.h"

NS_BEGIN(Engine)
class CModel;
class CTransform;
class CCell;

struct A_StarNode
{
	// 현재 내가 이동하는 거리의 코스트
	_float					fHeuristicCost;
	// 목표지점까지 이동했을때 나올 거리 코스트
	_float					fManhaattanDistance;
	// 위 두개의 값을 더해 나타낼 최종 거리 코스트
	_float					fTotalCost;

	_Int					iCellIndex;
	//부모 노드의 인덱스
	_Int					ParentIndex;
};


struct CompareNode {
	bool operator()(const A_StarNode& a, const A_StarNode& b) const {
		return a.fTotalCost > b.fTotalCost; // fCost가 작을수록 우선
	}
};


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
	virtual HRESULT					Initialize_Prototype(const CModel* pMapModel, _uInt iMeshNum);
	virtual HRESULT					Initialize(void* pArg);

	void							Update(_matrix WorldMatrix);
	_bool							IsMove(_vector vPosition);

	void							ComputeHeight(CTransform* pTransform);
	_Int							Find_Cell(_vector vPos);

	void							ComputePathfindingAStar(_float3 vStartPoint, _float3 vTargetPoint, list<_float3>* PathList);
	
	//보이어 왓슨 알고리즘 구현부
	void							Bowyer_WatsonAlgorithm(const CModel* pMapModel, _uInt iMeshNum);

#ifdef _DEBUG
public:
	HRESULT							Export(const char* FilePath);
	HRESULT							Render(_float4 vColor, _bool DarwCurCell = false);
#endif

private:
	_Int							m_iCurrentCellIndex = { -1 };
	_float3							m_MinPoint = {};
	_float3							m_MaxPoint = {};

	static _float4x4				m_WorldMatrix;

	vector<CCell*>					m_Cells;
	vector<_uInt>					m_EdgeCellIndex = {};

	list<pair<_Int, _Int>>			m_PathCells;

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

	_float							Triarea2DCross(const _float3 a, const _float3 b, const _float3 c);
	void							SimpleFunnelAlgorithm(_vector vStartPoint, list<_float3>* PathList);

	NAVI_TRIANGLE					CreateSuperTriangle(_float3 vMin, _float3 vMax);

public:
	static CNavigation*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFiles);
	static CNavigation*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CModel* pModel, _uInt iNumMesh);

	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END