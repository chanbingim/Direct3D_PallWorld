#pragma once
#include "ProjectileObject.h"

NS_BEGIN(Client)
class CArrowProjectile : public CProjectileObject
{
protected:
	CArrowProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CArrowProjectile(const CArrowProjectile& rhs);
	virtual ~CArrowProjectile() = default;

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

private:
	HRESULT									ADD_Components();
	void									BeginOverlapEvent(_float3 vDir, CGameObject* pTarget);
	 
public:
	static		CArrowProjectile*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END