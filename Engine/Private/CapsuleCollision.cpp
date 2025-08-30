#include "CapsuleCollision.h"

CCapsuleCollision::CCapsuleCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CCollision(pDevice, pContext)
{
}

CCapsuleCollision::CCapsuleCollision(const CCapsuleCollision& rhs) :
	CCollision(rhs)
{
}

HRESULT CCapsuleCollision::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCapsuleCollision::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CCapsuleCollision* CCapsuleCollision::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CComponent* CCapsuleCollision::Clone(void* pArg)
{
	return nullptr;
}

void CCapsuleCollision::Free()
{
	__super::Free();
}
