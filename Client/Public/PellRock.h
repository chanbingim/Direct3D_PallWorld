#pragma once
#include "MiningObject.h"

NS_BEGIN(Client)
class CPellRock : public CMiningObject
{
protected:
	CPellRock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPellRock(const CPellRock& rhs);
	virtual ~CPellRock() = default;

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
	virtual		HRESULT						DeadFunction() override;
	virtual		HRESULT						HitBeginFunction(_float3 vDir, CGameObject* pGameObject) override;
	virtual		void						Damage(void* pArg, CActor* pDamagedActor);

private:
	HRESULT									ADD_Components();

public:
	static			CPellRock*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END