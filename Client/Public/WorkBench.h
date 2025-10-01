#pragma once
#include "Architecture.h"

NS_BEGIN(Client)
class CWorkBench final : public CArchitecture
{
protected:
	CWorkBench(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorkBench(const CWorkBench& rhs);
	virtual ~CWorkBench() = default;

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
	virtual void							HitOverlapFunction(_float3 vDir, CGameObject* pHitActor);

private:
	HRESULT									ADD_Components();

public:
	static			CWorkBench*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END