#include "NpcBody.h"

#include "GameInstance.h"

CNpcBody::CNpcBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CNpcBody::CNpcBody(const CNpcBody& rhs) :
    CPartObject(rhs)
{
}

HRESULT CNpcBody::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CNpcBody::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
	{

	}
	else
	{
		PARTOBJECT_DESC* Desc = static_cast<PARTOBJECT_DESC*>(pArg);
		m_SocketMatrix = Desc->SocketMatrix;
		m_UseSocketMatrixFlag = Desc->UseSocketMatrixFlag;
	}

	return S_OK;
}

void CNpcBody::Priority_Update(_float fDeletaTime)
{
}

void CNpcBody::Update(_float fDeletaTime)
{
}

void CNpcBody::Late_Update(_float fDeletaTime)
{
}

HRESULT CNpcBody::Render()
{
    return E_NOTIMPL;
}

void CNpcBody::PlayAnimation(const char* szAnimName, _bool bIsLoop)
{
	if (szAnimName)
	{
		m_iAnimIndex = m_pVIBufferCom->GetNumAnimation(szAnimName);
		m_bIsAnimLoop = bIsLoop;
	}
}

void CNpcBody::ResetCurrentAnimation()
{
	m_pVIBufferCom->ResetAnimation(0);
}

_bool CNpcBody::FinishedAnimation()
{
	return m_bIsAnimFinished;
}

CGameObject* CNpcBody::Clone(void* pArg)
{
    return nullptr;
}

void CNpcBody::Free()
{
	__super::Free();
}
