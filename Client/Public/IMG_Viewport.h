#pragma once
#include "ImgUIBase.h"

NS_BEGIN(Client)
class CImgManager;

class CIMG_Viewport : public CImgUIBase
{
private :
	CIMG_Viewport();
	virtual ~CIMG_Viewport() = default;

public :
	virtual HRESULT					Prototype_Initialize();
	virtual HRESULT					Initialize(void* pArg);

	virtual void					Update(_float fDeletaTime);

	void							Change_EditMode(EDIT_MODE eMode);
	const EDIT_MODE&				GetEditMode();

private :
	CImgManager*					m_pImgManager = nullptr;
	_float2							m_ViewFrameSize = {};
	_float2							m_CneterPos = {};

	EDIT_MODE						m_eEditMode = { EDIT_MODE::END };
	_char							m_szSelect[MAX_PATH] = {};
	const _char*					m_szModeName[ENUM_CLASS(EDIT_MODE::END)] = {};

private :
	void							DrawViewPortMenuBar();
	void							DrawGameView();

	void							IsViewportClicked();

public :
	static		CIMG_Viewport*		Create();
	virtual		void				Free() override;
};
NS_END