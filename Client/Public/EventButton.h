#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CEventButton : public CBackGround
{
private:
	CEventButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEventButton(const CEventButton& rhs);
	virtual ~CEventButton() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									Set_Text(const WCHAR* szText);
	void									Bind_Event(function<void()> Event);

protected:
	virtual		void						MouseHoverEnter();
	virtual		void						MouseHovering();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();
	virtual		void						MouseButtonPressed();
	virtual		void						MouseButtonUp();

private:
	CFontComponent*							m_pFontCom = nullptr;

	_float2									m_fFontPoint = { };
	_wstring								m_szViewName = {};

	function<void()>						m_ClickEvent = nullptr;

private:
	HRESULT									ADD_Components();

public:
	static		CEventButton*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END