#pragma once
#include "Enviormnent.h"

NS_BEGIN(Client)
class CTreeObject : public CEnviormnent
{
protected:
	CTreeObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTreeObject(const CTreeObject& rhs);
	virtual ~CTreeObject() = default;

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
	HRESULT									ADD_Components(_uInt iModelIndex);

public:
	static			CTreeObject*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END