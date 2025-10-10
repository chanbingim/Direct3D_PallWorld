#pragma once
#include "Npc.h"

NS_BEGIN(Client)
class CFemalePeople_1 final : public CNpc
{
protected:
	CFemalePeople_1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFemalePeople_1(const CFemalePeople_1& rhs);
	virtual ~CFemalePeople_1() = default;

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
	void									OverlapEvent(_float3 vDir, CGameObject* pHitActor);


	HRESULT									ADD_Components();
	HRESULT									ADD_PartObjects();
	HRESULT									Setup_NpcFsm();

public:
	static			CFemalePeople_1*			Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END