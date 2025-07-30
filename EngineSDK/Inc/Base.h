#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)
class ENGINE_DLL CBase abstract
{
protected :
	CBase() = default;
	virtual ~CBase() = default;

public :
	DWORD		AddRef();
	DWORD		Release();

private :
	_uInt		m_RefCount = {0};

public:
	virtual void Free();
};
NS_END