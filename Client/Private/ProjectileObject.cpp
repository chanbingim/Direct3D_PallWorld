#include "ProjectileObject.h"
#include "GameInstance.h"

CProjectileObject::CProjectileObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPartObject(pDevice, pContext)
{
}

CProjectileObject::CProjectileObject(const CProjectileObject& rhs) :
	CPartObject(rhs)
{
}

HRESULT CProjectileObject::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CProjectileObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	PROJECTILE_DESC* pProjectileDesc = static_cast<PROJECTILE_DESC*>(pArg);
	m_fThorwDireaction = pProjectileDesc->vDireaction;
	m_fThorowSpeed = pProjectileDesc->vThrowSpeed;

	_vector vPosition = XMLoadFloat3(&pProjectileDesc->vPosition);
	_vector vDireaction = XMLoadFloat3(&m_fThorwDireaction);
	m_pTransformCom->LookAt(vPosition + vDireaction);

	return S_OK;
}

void CProjectileObject::Priority_Update(_float fDeletaTime)
{

}

void CProjectileObject::Update(_float fDeletaTime)
{
}

void CProjectileObject::Late_Update(_float fDeletaTime)
{
}

HRESULT CProjectileObject::Render()
{
	return E_NOTIMPL;
}


CGameObject* CProjectileObject::Clone(void* pArg)
{
	return nullptr;
}

void CProjectileObject::Free()
{
	__super::Free();

	Safe_Release(m_pCollision);
}
