#include "PellBase.h"

#include "GameInstance.h"

CPellBase::CPellBase(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CMeshActor(pGraphic_Device, pDeviceContext)
{
   
}

CPellBase::CPellBase(const CPellBase& rhs) :
    CMeshActor(rhs)
{
}

HRESULT CPellBase::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPellBase::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CPellBase::Priority_Update(_float fDeletaTime)
{
}

void CPellBase::Update(_float fDeletaTime)
{
}

void CPellBase::Late_Update(_float fDeletaTime)
{
}

HRESULT CPellBase::Render()
{
    return E_NOTIMPL;
}

CGameObject* CPellBase::Clone(void* pArg)
{
    return nullptr;
}

void CPellBase::Free()
{
    __super::Free();
}
