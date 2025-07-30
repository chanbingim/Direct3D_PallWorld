#include "Base.h"

DWORD CBase::AddRef()
{
	return ++m_RefCount;
}

DWORD CBase::Release()
{
	if (0 == m_RefCount)
	{
		Free();

		delete this;
		return 0;
	}
	else
		return m_RefCount--;
}

void CBase::Free()
{

}
