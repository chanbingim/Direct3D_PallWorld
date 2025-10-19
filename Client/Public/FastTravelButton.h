#pragma once
#include "Button.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CFastTravelButton : public CButton
{
public :
	typedef struct FastTravelDesc : public GAMEOBJECT_DESC
	{
		const WCHAR*				szMapName;
		const WCHAR*				szShowFontText;

	}FAST_TRAVEL_DESC;

private:
	CFastTravelButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFastTravelButton(const CFastTravelButton& rhs);
	virtual ~CFastTravelButton() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;
	void									Bind_ClickEvent(function<void(const _wstring&)> ClickFunc);

protected :
	virtual			void					MouseButtonDwon() override;

private:
	_wstring								m_szMoveMap = {};
	CFontComponent*							m_pFontCom = nullptr;
	function<void(const _wstring&)>			m_ClickEventFunc = nullptr;

	_wstring								m_szFontText = {};
	_float2									m_vFontPos = {};

private:
	HRESULT									ADD_Components();

public:
	static		CFastTravelButton*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END