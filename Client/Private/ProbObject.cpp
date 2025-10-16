#include "ProbObject.h"

CProbObject::CProbObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CNoneAnimMesh(pDevice, pContext)
{
}

CProbObject::CProbObject(const CProbObject& rhs) :
    CNoneAnimMesh(rhs)
{
}

HRESULT CProbObject::Initalize_Prototype()
{

    return S_OK;
}

HRESULT CProbObject::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    return S_OK;
}

void CProbObject::Priority_Update(_float fDeletaTime)
{

}

void CProbObject::Update(_float fDeletaTime)
{
}

void CProbObject::Late_Update(_float fDeletaTime)
{

}

HRESULT CProbObject::Render()
{
    return S_OK;
}

CGameObject* CProbObject::Clone(void* pArg)
{
    return nullptr;
}

void CProbObject::Free()
{
    __super::Free();

    Safe_Release(m_pCollision);
}
