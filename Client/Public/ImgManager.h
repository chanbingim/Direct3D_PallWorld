#pragma once

#include "Client_Define.h"
#include "Client_ImgDefines.h"
#include "PaseDataHeader.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)
class CImgUIBase;

class CImgManager final : public CBase
{
	DECLARE_SINGLETON(CImgManager);

private:
	CImgManager() = default;
	virtual ~CImgManager() = default;

public:
	HRESULT			Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void			Update(_float fDeletaTime);
	void			Render_Begin();
	void			Render_End();

	void							ADD_SelectObject(CGameObject* SelectObj);
	void							Remove_SelectObject(CGameObject* SelectObj);

	const list<CGameObject*>* GetSelectObjects();
	void							ClearAllSelectObjects();

	HRESULT							ADD_IMG_UserInterface(const _wstring szTag, CImgUIBase* pUserInterface);
	CImgUIBase* Find_ImgUserInterface(const WCHAR* szUITag);

	void							SetPickingPoint(_float3 vPickingPoint);
	_float3							GetPickingPoint() { return m_MousePickingPoint; }

private:
	// Img gui를 여기다가 만들어서 보관하고 그걸 통해서 제어
	_float3									m_MousePickingPoint = {};

	unordered_map<_wstring, CImgUIBase*>	m_ImgUIMap = {};
	ImGuiWindowFlags						m_ImGuiWindowFlags = {};
	list<CGameObject*>						m_SelectList = {};

private:
	HRESULT			Default_Setting(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT			Setting_Img_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void			CreateDockSpace();
	void			DarwMenuBar();

	HRESULT			SaveLevel();
	HRESULT			LoadObject(const char* FilePath, const WCHAR* ObjectTag);
	HRESULT			LoadEnvObject(const char* FilePath, const WCHAR* ObjectTag);

	void			SaveFile(const char* FilePath, list<SAVE_LEVEL_DESC>& SaveData);
	void			ReadFile(const char* FilePath, list<SAVE_LEVEL_DESC>& pOutList);

public:
	virtual			void			Free() override;

};
NS_END