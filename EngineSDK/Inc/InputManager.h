#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CInputManager final : public CBase
{
private : 
	CInputManager();
	virtual ~CInputManager() = default;

public :
	HRESULT		Initialize(HINSTANCE Instance, HWND hWnd, _uInt iWidth, _uInt iHegiht);
	
	void		UpdateKeyFrame();

	//마우스 입력 0 : 왼쪽 , 1 : 오른쪽 , 2 가운데 버튼
	_bool		KeyDown(KEY_INPUT eType, _uInt KeyState);
	_bool		KeyPressed(KEY_INPUT eType, _uInt KeyState);
	_bool		KeyUp(KEY_INPUT eType, _uInt KeyState);

	// 0 : X 축    1  : Y 축    2 : Z축
	LONG		GetMouseAxis(_uInt iAxis);

private :
	LPDIRECTINPUT8				m_pDirectInput = nullptr;
	LPDIRECTINPUTDEVICE8		m_pKeyboard = nullptr;
	LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;

	//키보드 입력 상태 배열
	BYTE						m_CurKeyState[MAX_KEY] = {};
	BYTE						m_PreKeyState[MAX_KEY] = {};

	//마우스 입력 상태 배열
	DIMOUSESTATE				m_DIMouseSate = {};
	BYTE						m_PreMouseState[MAX_MOUSEKEY] = {};

public :
	static		CInputManager*		Create(HINSTANCE Instance,  HWND hWnd, _uInt iWidth, _uInt iHegiht);
	virtual		void				Free() override;
};
NS_END