#include "Enviormnent.h"

#include "GameInstance.h"

CEnviormnent::CEnviormnent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CNoneAnimMesh(pDevice, pContext)
{
}

CEnviormnent::CEnviormnent(const CEnviormnent& rhs) :
    CNoneAnimMesh(rhs)
{
}

HRESULT CEnviormnent::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEnviormnent::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CEnviormnent::Priority_Update(_float fDeletaTime)
{
}

void CEnviormnent::Update(_float fDeletaTime)
{
}

void CEnviormnent::Late_Update(_float fDeletaTime)
{
}

HRESULT CEnviormnent::Render()
{
    return S_OK;
}

CGameObject* CEnviormnent::Clone(void* pArg)
{
    return nullptr;
}

void CEnviormnent::Free()
{
    __super::Free();
}
