#include "PellBase.h"

#include "GameInstance.h"

CPellBase::CPellBase(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CAnimMeshActor(pGraphic_Device, pDeviceContext)
{
   
}

CPellBase::CPellBase(const CPellBase& rhs) :
    CAnimMeshActor(rhs)
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

HRESULT CPellBase::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();
    m_pBoneMatrixEffect = m_pShaderCom->GetVariable("g_BoneMatrices")->AsMatrix();

    return S_OK;
}

CGameObject* CPellBase::Clone(void* pArg)
{
    return nullptr;
}

void CPellBase::Free()
{
    __super::Free();
}
