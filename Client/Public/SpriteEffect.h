#pragma once
#include "Client_Define.h"
#include "GameObject.h"

NS_BEGIN(Client)
class CSpriteEffect abstract : public CGameObject
{
protected:
	CSpriteEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpriteEffect(const CSpriteEffect& rhs);
	virtual ~CSpriteEffect() = default;

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
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END