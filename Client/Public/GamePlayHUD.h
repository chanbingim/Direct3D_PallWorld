#pragma once

#include "Client_Define.h"
#include "HeadUpDisplay.h"

NS_BEGIN(Client)
class CSelectUI;
class CAimInterface;
class CCreateMenu;
class CInGameMenu;

class CGamePlayHUD final : public CHeadUpDisplay
{
protected:
	CGamePlayHUD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CGamePlayHUD() = default;

public:
	virtual HRESULT				Initialize() override;
	virtual void				Update(_float fDeletaTime) override;

	void						SetVisibleSelectUI(VISIBILITY eVisible);
	CSelectUI*					GetSelectUI();

private :
	CInGameMenu*				m_pInGameMenu = nullptr;
	CAimInterface*				m_pCrossHair = nullptr;
	CCreateMenu*				m_pCreateMenu = nullptr;

	CSelectUI*					m_pSelectUI = nullptr;
	_bool						m_Visible = false;

private:
	HRESULT						ADD_UserInterface();

	void						UIKeyInput();
	void						Change_ViewUI();

	void						FoucusInUserInterface(_bool bFlag);
	void						ALL_PopUp_UnActive();

public:
	static	CGamePlayHUD*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;
};
NS_END