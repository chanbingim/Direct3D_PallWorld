#pragma once

#include "Client_Define.h"
#include "HeadUpDisplay.h"

NS_BEGIN(Client)
class CSelectUI;
class CAimInterface;
class CCreateMenu;
class CInGameMenu;
class CBossHealthBar;
class CWorkBenchCreateUI;
class CCreateToolTipUI;
class CDiallogUI;

class CGamePlayHUD final : public CHeadUpDisplay
{
protected:
	CGamePlayHUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CGamePlayHUD() = default;

public:
	virtual HRESULT				Initialize() override;
	virtual void				Update(_float fDeletaTime) override;

	void						SetVisibleSelectUI(VISIBILITY eVisible);
	HRESULT						ActivePopUpUserInterface(_uInt iID);
	HRESULT						UnActivePopUpUserInterface(_uInt iID);
	
	HRESULT						ActivePreviewUserInterface(_uInt iID);
	HRESULT						UnActivePreviewUserInterface(_uInt iID);
	
	CUserInterface*				GetPopUpUserInterface(_uInt iPopupID);
	CUserInterface*				GetPreViewUserInterface(_uInt iPopupID);

	void						SetBossHealthBar(void* PellInfo);
	void						HiddenBossHealthBar();

	CSelectUI*					GetSelectUI();
	void						UnActiveAllPopUp();
	void						UnActiveAllPreView();

private :
	CInGameMenu*					m_pInGameMenu = nullptr;
	CBossHealthBar*					m_pBossHealthbar = nullptr;

	CCreateMenu*					m_pCreateMenu = nullptr;
	CWorkBenchCreateUI*				m_pWorkBenchCreateMenu = nullptr;

	CSelectUI*						m_pSelectUI = nullptr;
	CCreateToolTipUI*				m_pCreateToolTip = nullptr;

	map<_uInt, CUserInterface*>		m_PopupUIs;
	map<_uInt, CUserInterface*>		m_PreviewUIs;

	_bool							m_Visible = false;

private:
	HRESULT						ADD_UserInterface();
	HRESULT						ADD_PreviewUserInterface();

	void						UIKeyInput();
	void						Change_ViewUI();

	void						FoucusInUserInterface(_bool bFlag);

public:
	static	CGamePlayHUD*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;
};
NS_END