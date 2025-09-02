#include "PellBody.h"

#include "GameInstance.h"

CPellBody::CPellBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CPellBody::CPellBody(const CPellBody& rhs) :
    CPartObject(rhs)
{
}

HRESULT CPellBody::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPellBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CPellBody::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CPellBody::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CPellBody::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CPellBody::Render()
{
    return S_OK;
}

CGameObject* CPellBody::Clone(void* pArg)
{
    return nullptr;
}

void CPellBody::Free()
{
    __super::Free();
}
