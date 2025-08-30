#include "SphereCollision.h"

CSphereCollision::CSphereCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CCollision(pDevice, pContext)
{
}

CSphereCollision::CSphereCollision(const CSphereCollision& rhs) :
	CCollision(rhs)
{
}

HRESULT CSphereCollision::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSphereCollision::Initialize(void* pArg)
{
	return S_OK;
}

CSphereCollision* CSphereCollision::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSphereCollision* pSphereCollision = new CSphereCollision(pDevice, pContext);
	if (FAILED(pSphereCollision->Initialize_Prototype()))
	{
		Safe_Release(pSphereCollision);
		MSG_BOX("CREATE FAIL : SPEHRE COLLISION");
	}
	return pSphereCollision;
}

CComponent* CSphereCollision::Clone(void* pArg)
{
	CSphereCollision* pSphereCollision = new CSphereCollision(*this);
	if (FAILED(pSphereCollision->Initialize(pArg)))
	{
		Safe_Release(pSphereCollision);
		MSG_BOX("CLONE FAIL : SPEHRE COLLISION");
	}
	return pSphereCollision;
}

void CSphereCollision::Free()
{
	__super::Free();
}
