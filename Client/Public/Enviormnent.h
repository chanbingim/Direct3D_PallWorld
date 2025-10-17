#pragma once
#include "WorkAbleObject.h"

NS_BEGIN(Engine)
class CCollision;
NS_END

NS_BEGIN(Client)
class CDropComponent;

class CEnviormnent abstract : public CWorkAbleObject
{
public :
	enum class ENVIORNMENT_TYPE { TREE, ROCK, PELLSTONE, END};
	
	typedef struct Enviornment_Dsec : GAMEOBJECT_DESC
	{
		_uInt			iModelIndex;

	}ENVIORMNENT_DESC;

protected :
	CEnviormnent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnviormnent(const CEnviormnent& rhs);
	virtual ~CEnviormnent() = default;

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
	virtual		void						Damage(void* pArg, CActor* pDamagedActor);

protected:
	CCollision*								m_pCollision = nullptr;
	CDropComponent*							m_pDropComponent = nullptr;

	_float									m_fHealth = {};

protected :
	virtual		HRESULT						HitBeginFunction(_float3 vDir, CGameObject* pGameObject);
	virtual		HRESULT						HitOverlapFunction(_float3 vDir, CGameObject* pGameObject);
	virtual		HRESULT						DeadFunction() override;

public:
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END