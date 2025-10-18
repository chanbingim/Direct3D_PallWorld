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
	void							Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation*					GetNavimesh();

	// 여기다가 이제 쿼드 트리형태로 셀 가져오는거
	// 건설 가능한 지형인지 탐색 등 구현할 예정
	// 나중에 레벨 내에서 테레인 이동을 넣을지도 모름
	HRESULT							ADD_Navigation(const WCHAR* szMapTag, const char* pFilePath);
	HRESULT							Remove_Navigation(const WCHAR* szMapTag);
	CNavigation*					Find_MapNavigation(const WCHAR* szMapTag);

private :
	ID3D11Device*						m_pDevice = nullptr;
	ID3D11DeviceContext*				m_pContext = nullptr;

	_int2								m_TerrianSize = {};
	_Int								m_TerrianSelectIndex = {};

	CDefaultMap*						m_DefaultMap;
	map<const _wstring, CNavigation*>	m_pMapNavigation;

public :
	virtual			void			Free() override;

};
NS_END