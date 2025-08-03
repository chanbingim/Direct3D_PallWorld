#pragma once
#include <Windows.h>
#include <process.h>

namespace Client
{
	const unsigned int	g_iWinSizeX = 1280;
	const unsigned int	g_iWinSizeY = 720;

	const unsigned int	g_iHalfWinSizeX = UINT(g_iWinSizeX * 0.5f);
	const unsigned int	g_iHalfWinSizeY = UINT(g_iWinSizeY * 0.5f);

	enum class LEVEL { STATIC, LOGO, GAMEPLAY, LOADING, END };
}

extern HWND					g_hWnd;
extern HINSTANCE			g_hInstance;

using namespace Client;