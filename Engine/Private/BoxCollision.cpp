#include "BoxCollision.h"

CBoxCollision::CBoxCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CCollision(pDevice, pContext)
{
}

CBoxCollision::CBoxCollision(const CBoxCollision& rhs) :
    CCollision(rhs)
{
}

HRESULT CBoxCollision::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBoxCollision::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

CBoxCollision* CBoxCollision::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBoxCollision* pBoxCollision = new CBoxCollision(pDevice, pContext);
    if (FAILED(pBoxCollision->Initialize_Prototype()))
    {
        Safe_Release(pBoxCollision);
        MSG_BOX("CREATE FAIL : BOX COLLISION");
    }
    return pBoxCollision;
}

CComponent* CBoxCollision::Clone(void* pArg)
{
    CBoxCollision* pBoxCollision = new CBoxCollision(*this);
    if (FAILED(pBoxCollision->Initialize(pArg)))
    {
        Safe_Release(pBoxCollision);
        MSG_BOX("CLONE FAIL : BOX COLLISION");
    }
    return pBoxCollision;
}

void CBoxCollision::Free()
{
    __super::Free();
}