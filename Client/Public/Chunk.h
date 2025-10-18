#pragma once
#include "Client_Define.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CNavigation;
class CTransform;
class CGameInstance;
class CCollision;
NS_END

NS_BEGIN(Client)
class CChunk : public CGameObject
{
public :
	typedef struct ChunkDesc : public GAMEOBJECT_DESC
	{
		const char*			NavigationFilePath;
	}CHUNK_DESC;

private :
	CChunk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CChunk() = default;

public :
	HRESULT							Initialize(void* pArg);

	_bool							IsIn(_float3	vPosition);
	CNavigation*					GetChunckNavigation();

private :
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pContext = nullptr;
	CGameInstance*					m_pGameInstance = nullptr;

	CNavigation*					m_pNavigation = nullptr;
	CTransform*						m_pTransform = nullptr;
	CCollision*						m_pCollision = nullptr;

private :
	HRESULT							ADD_Component(CHUNK_DESC* pDesc);

public :
	static		CChunk*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual		void				Free() override;
};
NS_END