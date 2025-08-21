#include "MeshActor.h"

#include "GameInstance.h"

CMeshActor::CMeshActor(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CActor(pGraphic_Device, pDeviceContext)
{
}

CMeshActor::CMeshActor(const CMeshActor& rhs) :
     CActor(rhs)
{
}

HRESULT CMeshActor::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CMeshActor::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CMeshActor::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CMeshActor::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CMeshActor::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CMeshActor::Render()
{

    return S_OK;
}

HRESULT CMeshActor::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pSRVEffect = m_pShaderCom->GetVariable("g_Texture")->AsShaderResource();

    return S_OK;
}

HRESULT CMeshActor::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    ID3D11ShaderResourceView* pResourceVeiw = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);

    return S_OK;
}


HRESULT CMeshActor::ADD_Components()
{
    return S_OK;
}

CGameObject* CMeshActor::Clone(void* pArg)
{
    return nullptr;
}

void CMeshActor::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
}
