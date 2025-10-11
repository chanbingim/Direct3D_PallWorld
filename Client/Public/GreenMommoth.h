#pragma once
#include "PellBase.h"

NS_BEGIN(Client)
class CDefenceBossComponent;
class CAiSenceComponent;

class CGreenMommoth : public CPellBase
{
protected:
	CGreenMommoth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGreenMommoth(const CGreenMommoth& rhs);
	virtual ~CGreenMommoth() = default;

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

private :
	CDefenceBossComponent*					m_pDefecnBossCom = nullptr;
	CAiSenceComponent*						m_pAiSenceCom = nullptr;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_PartObjects();

private :
	HRESULT									Setup_PellFsm();

	void									TargetSearchBegin(CGameObject* pSearchObject);
	void									TargetLost(CGameObject* pSearchObject);

	void									OverlapEvent(_float3 vDir, CGameObject* pHitObject);

public:
	static			CGreenMommoth*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END