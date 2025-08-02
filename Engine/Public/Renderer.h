#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CRenderer final : public CBase
{
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT		Initialize();
	HRESULT		Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pRenderObject);
	void		Render();

private:
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };

	//랜더 스테이트
	ID3D11BlendState*					m_pAlphaBlendState = { nullptr };

	list<class CGameObject*>			m_RenderObjects[ENUM_CLASS(RENDER::END)];

private:
	HRESULT				Create_BlendState();

	void				Render_Priority();
	void				Render_NonBlend();
	void				Render_WorldUI();
	void				Render_Blend();
	void				Render_ScreenUI();

public:
	static CRenderer*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void		Free();
};
NS_END