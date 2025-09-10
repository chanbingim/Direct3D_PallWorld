#include "Recovery.h"

CRecovery::CRecovery(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CRecovery::CRecovery(const CRecovery& Prototype) :
	CComponent(Prototype)
{
}

HRESULT CRecovery::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRecovery::Initialize(void* pArg)
{
	RECOVERY_DESC* pDesc = static_cast<RECOVERY_DESC*>(pArg);
	m_pOwner = pDesc->pOwner;
	m_fRecoveryTime = pDesc->fRecoveryTime;
	m_fPerTickRecovery = pDesc->fPerTickRecovery;

	return S_OK;
}

_float CRecovery::Update(_float fTimeDeleta)
{
	_float ReturnValue = {};

	m_fAccTime += fTimeDeleta;
	if (m_fAccTime >= m_fRecoveryTime)
	{
		m_fAccTime = 0;
		ReturnValue = m_fPerTickRecovery;
	}

	return ReturnValue;
}

void CRecovery::ResetComponent()
{
	m_fAccTime = 0.f;
}

CRecovery* CRecovery::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRecovery* pRecovery = new CRecovery(pDevice, pContext);
	if (FAILED(pRecovery->Initialize_Prototype()))
	{
		Safe_Release(pRecovery);
		MSG_BOX("CREATE FAIL : RECOVERY");
	}
	return pRecovery;
}

CComponent* CRecovery::Clone(void* pArg)
{
	CRecovery* pRecovery = new CRecovery(*this);
	if (FAILED(pRecovery->Initialize(pArg)))
	{
		Safe_Release(pRecovery);
		MSG_BOX("CLONE FAIL : RECOVERY");
	}
	return pRecovery;
}

void CRecovery::Free()
{
	__super::Free();
}