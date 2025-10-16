#include "WorkAbleObject.h"

#include "GameInstance.h"

CWorkAbleObject::CWorkAbleObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CNoneAnimMesh(pDevice, pContext)
{
}

CWorkAbleObject::CWorkAbleObject(const CWorkAbleObject& rhs) :
    CNoneAnimMesh(rhs),
    m_eWorkType(rhs.m_eWorkType)
{
}

HRESULT CWorkAbleObject::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWorkAbleObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CWorkAbleObject::Priority_Update(_float fDeletaTime)
{
}

void CWorkAbleObject::Update(_float fDeletaTime)
{
}

void CWorkAbleObject::Late_Update(_float fDeletaTime)
{
}

HRESULT CWorkAbleObject::Render()
{
    return S_OK;
}



HRESULT CWorkAbleObject::DeadFunction()
{
    return S_OK;
}

CGameObject* CWorkAbleObject::Clone(void* pArg)
{
    return nullptr;
}

void CWorkAbleObject::Free()
{
    __super::Free();
}
