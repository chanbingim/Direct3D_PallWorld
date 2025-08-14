#pragma once
#include "ImgUIBase.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)
class CImgManager;
class CIMG_Component;

class CIMG_Inspector : public CImgUIBase
{
private:
	CIMG_Inspector();
	virtual ~CIMG_Inspector() = default;

public:
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;

private :
	CImgManager*											m_pImgManager = nullptr;
	CGameObject*											m_pSelectObj = nullptr;
	unordered_map<_wstring, pair<CIMG_Component*,_bool>>	m_InspectorUIMap;

private :
	HRESULT							Default_Setting();
	HRESULT							ADD_InspectorUI(const WCHAR* szTag, CIMG_Component* pUserInterface);

	void							Update_UIState();

public:
	static	CIMG_Inspector*			Create();
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;

};
NS_END