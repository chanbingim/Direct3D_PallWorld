#pragma once

#include "../Default/framework.h"

namespace Client
{
	const unsigned int	g_iWinSizeX = 1280;
	const unsigned int	g_iWinSizeY = 720;

	const unsigned int	g_iHalfWinSizeX = UINT(g_iWinSizeX * 0.5f);
	const unsigned int	g_iHalfWinSizeY = UINT(g_iWinSizeY * 0.5f);

	enum class LEVEL { STATIC, LOGO, GAMEPLAY, LOADING, END };
	enum class ITEM_TYPE { EQUIPMENT, CONSUM, ETC, END };
	enum class COSTUME_TYPE { HEAD, BODY, BOTTOM, WHOLE_BODY, SHOES,END};
}

extern HWND					g_hWnd;
extern HINSTANCE			g_hInstance;

#ifdef _DEBUG
extern	bool				g_ShowDebugWindow;
extern	HWND				g_hWnd_Debug;
extern	HINSTANCE			g_hInstance_Debug;
#endif // _DEBUG

using namespace Client;