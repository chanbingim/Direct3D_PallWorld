#include "AnimMeshActor.h"

#include "GameInstance.h"

CAnimMeshActor::CAnimMeshActor(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CActor(pGraphic_Device, pDeviceContext)
{
}

CAnimMeshActor::CAnimMeshActor(const CAnimMeshActor& rhs) :
     CActor(rhs)
{
}

HRESULT CAnimMeshActor::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CAnimMeshActor::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CAnimMeshActor::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CAnimMeshActor::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CAnimMeshActor::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CAnimMeshActor::Render()
{

    return S_OK;
}

void CAnimMeshActor::SetAnimIndex(_uInt iIndex)
{
    m_iAnimIndex = iIndex;
}

HRESULT CAnimMeshActor::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pSRVEffect = m_pShaderCom->GetVariable("g_Texture")->AsShaderResource();
    m_pBoneMatrixEffect = m_pShaderCom->GetVariable("g_BoneMatrices")->AsMatrix();

    return S_OK;
}

HRESULT CAnimMeshActor::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));
    m_pBoneMatrixEffect->SetMatrixArray(reinterpret_cast<const float*>(m_pVIBufferCom->GetBoneMatrices(iMeshIndex)), 0, m_pVIBufferCom->GetMeshNumBones(iMeshIndex));

    ID3D11ShaderResourceView* pResourceVeiw = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);

    return S_OK;
}


HRESULT CAnimMeshActor::ADD_Components()
{
    return S_OK;
}

CGameObject* CAnimMeshActor::Clone(void* pArg)
{
    return nullptr;
}

void CAnimMeshActor::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
}
