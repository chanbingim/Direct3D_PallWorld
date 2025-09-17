#include "Actor.h"

CActor::CActor(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CGameObject(pGraphic_Device, pDeviceContext)
{
}

CActor::CActor(const CActor& rhs) :
	CGameObject(rhs)
{
}

HRESULT CActor::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CActor::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CActor::Priority_Update(_float fDeletaTime)
{
}

void CActor::Update(_float fDeletaTime)
{
}

void CActor::Late_Update(_float fDeletaTime)
{
	__super::Late_Update(fDeletaTime);
}

HRESULT CActor::Render()
{
	return S_OK;
}

void CActor::Damage(void* pArg, CActor* pDamagedActor)
{
}

CGameObject* CActor::Clone(void* pArg)
{
	return nullptr;
}

void CActor::Free()
{
	__super::Free();
}
