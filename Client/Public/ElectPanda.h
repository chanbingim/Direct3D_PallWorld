#pragma once
#include "PellBase.h"

NS_BEGIN(Client)
class CElectPanda : public CPellBase
{
protected:
	CElectPanda(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CElectPanda(const CElectPanda& rhs);
	virtual ~CElectPanda() = default;

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
	// 전투를 위한 전투 기능
	virtual		void						CombatAction(_float fDeletaTime, CGameObject* pTarget);
	virtual		void						OverlappingEvent(_float3 vDir, CGameObject* pHitObject) override;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_PartObjects();
	HRESULT									Setup_PellFsm();

	void									OverlapEvent(_float3 vDir, CGameObject* pHitObject);

	
public:
	static			CElectPanda*			Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END