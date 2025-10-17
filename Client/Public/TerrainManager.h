#pragma once
#include "Client_Define.h"
#include "Base.h"

#ifdef _DEBUG
#include "NavigationStruct.h"
#endif // _DEBUG

NS_BEGIN(Engine)
class CComponent;
class CNavigation;
NS_END

NS_BEGIN(Client)
class CDefaultMap;

class CTerrainManager : public CBase
{
	DECLARE_SINGLETON(CTerrainManager);
public :
	typedef struct		Terrain_Desc
	{
		_int2				TerrianSize;
		const char*			pFileData;

	}TERRIAN_DESC;

private :
	CTerrainManager();
	virtual ~CTerrainManager() = default;

public :
	void							Initialize(void* pArg);

	HRESULT							CreateTerrian(void* pArg);

	CNavigation*					GetNavimesh();

	// 여기다가 이제 쿼드 트리형태로 셀 가져오는거
	// 건설 가능한 지형인지 탐색 등 구현할 예정
	// 나중에 레벨 내에서 테레인 이동을 넣을지도 모름
#ifdef _DEBUG
	void							GetAllNaviMeshTriangle(list<NAVI_TRIANGLE>* pOut);
#endif // _DEBUG

private :
	_int2							m_TerrianSize = {};
	_Int							m_TerrianSelectIndex = {};
	CDefaultMap*					m_DefaultMap;

private :
	HRESULT							LoadTerrianData(const char* pFileData);

public :
	virtual			void			Free() override;

};
NS_END