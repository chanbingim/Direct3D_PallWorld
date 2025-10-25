#pragma once

#include "../Default/framework.h"

namespace Client
{
#define GRAVITY 9.8f

	const unsigned int	g_iWinSizeX = 1280;
	const unsigned int	g_iWinSizeY = 720;

	const unsigned int	g_iHalfWinSizeX = UINT(g_iWinSizeX * 0.5f);
	const unsigned int	g_iHalfWinSizeY = UINT(g_iWinSizeY * 0.5f);

	enum class LEVEL { EDITOR, STATIC, LOGO, GAMEPLAY, LOADING, END };

	enum class COSTUME_TYPE { HEAD, BODY, BOTTOM, WHOLE_BODY, SHOES,END};
	enum class DIRECTION { FRONT, LEFT, RIGHT, BACK, END};
	enum class ACTOR_TEAM { FRENDLY, NEUTRAL, ENEMY, BOSS, END };
}

extern HWND					g_hWnd;
extern HINSTANCE			g_hInstance;
extern	bool				g_GameWindowFocus;

#ifdef _DEBUG
extern	bool				g_ShowDebugWindow;
extern	HWND				g_hWnd_Debug;
extern	HINSTANCE			g_hInstance_Debug;
#endif // _DEBUG

using namespace Client;