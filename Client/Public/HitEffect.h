#pragma once
#include "SpriteEffect.h"

NS_BEGIN(Client)
class CHitEffect : public CSpriteEffect
{
protected:
	CHitEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHitEffect(const CHitEffect& rhs);
	virtual ~CHitEffect() = default;

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

protected:
	_float									m_fLifeTime = {};
	_float									m_fAccTime = {};

	list<CGameObject*>						m_pEffects = {};

public:
	virtual			CHitEffect*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END