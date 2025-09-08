#include "Navigation.h"
#include "Cell.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs) : 
	CComponent(rhs)
{
}

HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationDataFiles)
{
	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFiles)
{
	CNavigation* pNavigation = new CNavigation(pDevice, pContext);
	if (FAILED(pNavigation->Initialize_Prototype(pNavigationDataFiles)))
	{
		Safe_Release(pNavigation);
		MSG_BOX("CREATE FAIL : NAVIGATION");
	}
	return pNavigation;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pNavigation = new CNavigation(*this);
	if (FAILED(pNavigation->Initialize(pArg)))
	{
		Safe_Release(pNavigation);
		MSG_BOX("CLONE FAIL : NAVIGATION");
	}
	return pNavigation;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& iter : m_Cells)
		Safe_Release(iter);

}
