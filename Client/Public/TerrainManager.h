#pragma once
#include "Client_Define.h"
#include "Base.h"

NS_BEGIN(Client)
class CTerrain;

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
	
	HRESULT							ADD_Terrian(CTerrain* pTerrian);
	_bool							IsMove(_float3 vPosition);

	// 여기다가 이제 쿼드 트리형태로 셀 가져오는거
	// 건설 가능한 지형인지 탐색 등 구현할 예정


private :
	_int2							m_TerrianSize = {};
	vector<CTerrain*>				m_Terrians;

private :
	HRESULT							LoadTerrianData(const char* pFileData);

public :
	virtual			void			Free() override;

};
NS_END