#pragma once
#include "Architecture.h"

NS_BEGIN(Engine)
class CLight;
NS_END

NS_BEGIN(Client)
class CSunLight;

class CTouchLamp final : public CArchitecture
{
private :
	CTouchLamp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTouchLamp(const CTouchLamp& rhs);
	virtual ~CTouchLamp() = default;

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
	virtual		void						ArchitectureAction() override;

protected:
	virtual void							HitOverlapFunction(_float3 vDir, CGameObject* pHitActor);

private:
	CLight*									m_pTouchLight = nullptr;
	const CSunLight*						m_pSunLight = nullptr;

	_float									m_fLightLifeTime = {};
	_bool									m_bIsLight = false;

private:
	HRESULT									ADD_Components();

public:
	static			CTouchLamp*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END