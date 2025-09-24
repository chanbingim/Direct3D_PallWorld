#pragma once
#include "PellBase.h"

NS_BEGIN(Client)
class CDororong : public CPellBase
{
protected:
	CDororong(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CDororong(const CDororong& rhs);
	virtual ~CDororong() = default;

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
	virtual		void						CombatAction(CGameObject* pTarget);

private :
	_float3									m_HitReflectionDir = {};


private:
	HRESULT									ADD_Components();
	HRESULT									ADD_PartObjects();
	HRESULT									Setup_PellFsm();

	void									OverlapEvent(_float3 vDir, CGameObject* pHitObject);
	void									StunAction(_float fDeletaTime, _bool bIsStunEnd);

public:
	static			CDororong*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END