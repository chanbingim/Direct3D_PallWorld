#pragma once

#include "Client_Define.h"
#include "Client_ImgDefines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)
class CImgUIBase;

class CImgManager final : public CBase
{
	DECLARE_SINGLETON(CImgManager);

private :
	CImgManager() = default;
	virtual ~CImgManager() = default;

public :
	HRESULT			Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void			Update(_float fDeletaTime);
	void			Render();

	void							ADD_SelectObject(CGameObject* SelectObj);
	void							Remove_SelectObject(CGameObject* SelectObj);

	const list<CGameObject*>*		GetSelectObjects();
	void							ClearAllSelectObjects();

	HRESULT							ADD_IMG_UserInterface(const _wstring szTag, CImgUIBase* pUserInterface);

private :
	// Img gui를 여기다가 만들어서 보관하고 그걸 통해서 제어
	unordered_map<_wstring, CImgUIBase*>	m_ImgDebugMap = {};
	ImGuiWindowFlags						m_ImGuiWindowFlags = {};
	list<CGameObject*>						m_SelectList = {};

private :
	HRESULT			Default_Setting(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT			Setting_Img_UI();

	void			CreateDockSpace();
	void			DarwMenuBar();

public :
	virtual			void			Free() override;

};
NS_END