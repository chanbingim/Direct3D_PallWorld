#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CShader;
class CGameObject;
class CComponent;
class CVIBuffer_Rect;
class CGameInstance;

class CRenderer final : public CBase
{
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT		Initialize();
	HRESULT		Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pRenderObject);
	void		Render();

#ifdef _DEBUG
	void		ToggleRenderDebug();
	HRESULT		Add_DebugComponent(CComponent* pDebugCom);
#endif

private:
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };
	CGameInstance*						m_pGameInstance = nullptr;
	list<CGameObject*>					m_RenderObjects[ENUM_CLASS(RENDER::END)];
	ID3D11DepthStencilView*				m_pShadowDSV = { nullptr };

#ifdef _DEBUG
private:
	list<class CComponent*>				m_DebugComponents;
#endif

private:
	CShader*							m_pShader = { nullptr };
	CVIBuffer_Rect*						m_pVIBuffer = { nullptr };

private:
	_bool								m_bIsShowRenderTarget = false;
	_float4x4							m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};


private:
	void								Render_Priority();

	//디퍼드 셰이더를 위한 과정 4가지이다.
	void								Render_Shadow();
	void								Render_NonBlend();
	void								Render_LightAcc();
	void								Render_Combined();
	void								Render_NonLight();
	void								Render_Blur();

	void								Render_WorldUI();
	void								Render_Blend();
	void								Render_ScreenUI();

	/* 후처리를 하기위해 텍스처 생성 */
	void								DrawPosTex();

	HRESULT								Ready_DepthStencilView(_uInt iSizeX, _uInt iSizeY);

#ifdef _DEBUG
private:
	void Render_Debug();
#endif


public:
	static CRenderer*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void		Free();
};
NS_END