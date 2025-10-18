#include "MiningObject.h"

#include "PellBoxManager.h"

CMiningObject::CMiningObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CEnviormnent(pDevice, pContext)
{
}

CMiningObject::CMiningObject(const CMiningObject& rhs) :
	CEnviormnent(rhs)
{
}

HRESULT CMiningObject::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	m_fCompleteTime = 3.0f;
	return S_OK;
}

HRESULT CMiningObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMiningObject::Priority_Update(_float fDeletaTime)
{
}

void CMiningObject::Update(_float fDeletaTime)
{
	if (!m_bIsCompleted)
	{
		m_fAccTime += fDeletaTime * m_fWorkSpeed;
		if (1.f <= m_fAccTime)
		{
			m_fAccComplteTime++;
			m_fAccTime = 0;
		}

		if (m_fAccComplteTime >= m_fCompleteTime)
		{
			m_bIsCompleted = true;
			DeadFunction();
		}
	}
}

void CMiningObject::Late_Update(_float fDeletaTime)
{
}

HRESULT CMiningObject::Render()
{
	return S_OK;
}

void CMiningObject::PlusWorkSpeed(_float fWorkSpeed)
{
	m_fWorkSpeed += fWorkSpeed;
}

void CMiningObject::SubWorkSpeed(_float fWorkSpeed)
{
	m_fWorkSpeed -= fWorkSpeed;
}

CGameObject* CMiningObject::Clone(void* pArg)
{
	return nullptr;
}

void CMiningObject::Free()
{
	__super::Free();
}
