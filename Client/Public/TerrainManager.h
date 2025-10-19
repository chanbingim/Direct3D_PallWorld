#pragma once
#include "Client_Define.h"
#include "Base.h"

NS_BEGIN(Engine)
class CNavigation;
class CTransform;
NS_END

NS_BEGIN(Client)
class CChunk;

class CTerrainManager : public CBase
{
	DECLARE_SINGLETON(CTerrainManager);
public :
	typedef struct		Chunk_Desc
	{
		_wstring			ChunkName;
		CChunk*				pChunk;

	}CHUNK_DESC;

private :
	CTerrainManager();
	virtual ~CTerrainManager() = default;

public :
	void							Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	// 여기다가 이제 쿼드 트리형태로 셀 가져오는거
	// 건설 가능한 지형인지 탐색 등 구현할 예정
	// 나중에 레벨 내에서 테레인 이동을 넣을지도 모름
	HRESULT							ADD_Navigation(const WCHAR* szMapTag, const char* pFilePath);
	HRESULT							Remove_Navigation(const WCHAR* szMapTag);
		
	_bool							UpdateChunk(const WCHAR* ChunkKey, _float3	vMovePoint);

	// 객체의 위치만 전달해주면 알아서 높이 계산해서 반환해주자
	// 이거 위치반환해주는게 제일 깔끔할거같음
	_bool							ComputeHieght(CTransform* pTransform, _float3* vOutPoint, _bool bIsUpdateCell);
	void							Find_Chunk(_float3 vPos, CHUNK_DESC*	pOutDesc);

private :
	ID3D11Device*						m_pDevice = nullptr;
	ID3D11DeviceContext*				m_pContext = nullptr;
	map<const _wstring, CChunk*>		m_pMapNavigation;

private :
	CChunk*								Find_MapNavigation(const WCHAR* szMapTag);

public :
	virtual			void			Free() override;

};
NS_END