#pragma once

#include "Client_Define.h"
#include "HeadUpDisplay.h"

NS_BEGIN(Client)
class CSelectUI;
class CAimInterface;
class CCreateMenu;
class CInGameMenu;
class CWorkBenchCreateUI;
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
	CUserInterface*				GetPopUpUserInterface(_uInt iPopupID);


	CSelectUI*					GetSelectUI();
	void						UnActiveAllPopUp();

private :
	CInGameMenu*				m_pInGameMenu = nullptr;
	CCreateMenu*				m_pCreateMenu = nullptr;
	CWorkBenchCreateUI*			m_pWorkBenchCreateMenu = nullptr;
	
	map<_uInt, CUserInterface*>	m_PopupUIs;
	CSelectUI*					m_pSelectUI = nullptr;
	_bool						m_Visible = false;

private:
	HRESULT						ADD_UserInterface();

	void						UIKeyInput();
	void						Change_ViewUI();

	void						FoucusInUserInterface(_bool bFlag);

public:
	static	CGamePlayHUD*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;
};
NS_END