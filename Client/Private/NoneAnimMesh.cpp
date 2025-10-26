#include "NoneAnimMesh.h"

#include "GameInstance.h"
#include "TerrainManager.h"

CNoneAnimMesh::CNoneAnimMesh(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CActor(pGraphic_Device, pDeviceContext)
{
}

CNoneAnimMesh::CNoneAnimMesh(const CNoneAnimMesh& rhs) :
    CActor(rhs)
{
}

HRESULT CNoneAnimMesh::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNoneAnimMesh::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CNoneAnimMesh::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CNoneAnimMesh::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CNoneAnimMesh::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CNoneAnimMesh::Render()
{
    return S_OK;
}

void CNoneAnimMesh::RefreshComputeHeight()
{
    auto pTerrainManager = CTerrainManager::GetInstance();

    _float3 vHegithPos = m_pTransformCom->GetPosition();
    pTerrainManager->ComputeHieght(m_pTransformCom, &vHegithPos, true);
    //m_pTransformCom->SetPosition(vHegithPos);
}

HRESULT CNoneAnimMesh::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();

    return S_OK;
}

HRESULT CNoneAnimMesh::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));
    m_pShaderCom->Bind_RawValue("g_fCamFar", &m_pGameInstance->GetCameraINFO().y, sizeof(_float));

    ID3D11ShaderResourceView* pResourceVeiw = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);
    
    ID3D11ShaderResourceView* pNormalRSV = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_NORMALS, 0, &pNormalRSV);
    if (pNormalRSV)
        m_pShaderCom->Bind_SRV("g_NormalTexture", pNormalRSV);

    return S_OK;
}

HRESULT CNoneAnimMesh::ADD_Components()
{
    return E_NOTIMPL;
}

CGameObject* CNoneAnimMesh::Clone(void* pArg)
{
    return nullptr;
}

void CNoneAnimMesh::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
}
