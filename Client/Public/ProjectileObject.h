#pragma once
#include "Client_Define.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CCollision;
NS_END

NS_BEGIN(Client)
class CProjectileObject : public CPartObject
{
public:
	typedef struct ProjectileDesc : GAMEOBJECT_DESC
	{
		_float3			vDireaction;
		_float			vThrowSpeed;
	}PROJECTILE_DESC;

protected :
	CProjectileObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CProjectileObject(const CProjectileObject& rhs);
	virtual ~CProjectileObject() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

protected :
	CCollision*								m_pCollision = nullptr;
	_bool									m_bIsPlayAnim = false;

	_float									m_fThorowSpeed = {};
	_float3									m_fThorwDireaction = {};

public:
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END