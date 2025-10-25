#pragma once
#include "SkillObjectBase.h"

NS_BEGIN(Client)
class CEarthquake : public CSkillObjectBase
{
private:
	CEarthquake(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEarthquake(const CEarthquake& rhs);
	virtual ~CEarthquake() = default;

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
	void									HitOverlapEvent(_float3 vDir, CGameObject* pGameObject) override;

private :
	_bool									m_bIsLerpEnd = false;
	_float									m_fAccTime = {};
	_float									m_fTotalTime = {};

	_float									m_vSpeed = {};
	_float3									m_vStartScale = {};
	_float3									m_vEndScale = {};

private:
	HRESULT									ADD_Components();


	void									Effect_Dead();


public:
	static			CEarthquake*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END