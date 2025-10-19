#include "InstanceModel.h"

#include "GameInstance.h"

#include "TerrainManager.h"
#include "Chunk.h"

CInstanceModel::CInstanceModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CGameObject(pDevice, pContext)
{
}

CInstanceModel::CInstanceModel(const CInstanceModel& rhs) :
    CGameObject(rhs)
{
}

HRESULT CInstanceModel::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CInstanceModel::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CInstanceModel::Priority_Update(_float fDeletaTime)
{
}

void CInstanceModel::Update(_float fDeletaTime)
{
}

void CInstanceModel::Late_Update(_float fDeletaTime)
{
}

HRESULT CInstanceModel::Render()
{
    return E_NOTIMPL;
}

void CInstanceModel::RefreshComputeHeight()
{
    D3D11_MAPPED_SUBRESOURCE SubResource = {};

    CTerrainManager::CHUNK_DESC ChunkDesc = {};
    CTerrainManager::GetInstance()->Find_Chunk(m_pTransformCom->GetPosition(), &ChunkDesc);

    CNavigation* pNaviMesh = nullptr;
    if (ChunkDesc.pChunk)
    {
        pNaviMesh = ChunkDesc.pChunk->GetChunckNavigation();
        _matrix vWorldMat = XMLoadFloat4x4(&m_pTransformCom->GetWorldMat());
        _uInt iNumInstance = m_pVIBufferCom->GetNumInstance();
        m_pVIBufferCom->Lock(&SubResource);;
        VTX_INSTANCE_DEFAULT_DESC* pVertices = static_cast<VTX_INSTANCE_DEFAULT_DESC*>(SubResource.pData);
        if (pVertices)
        {
            for (_uInt i = 0; i < iNumInstance; i++)
            {
                _float3 vHeightPos = { pVertices[i].vPosition.x,
                                       pVertices[i].vPosition.y,
                                       pVertices[i].vPosition.z };

                XMStoreFloat3(&vHeightPos, XMVector3TransformCoord(XMLoadFloat3(&vHeightPos), vWorldMat));
                pNaviMesh->ComputeHeight(&vHeightPos);
                XMStoreFloat3(&vHeightPos, XMVector3TransformCoord(XMLoadFloat3(&vHeightPos), XMMatrixInverse(nullptr, vWorldMat)));

                pVertices[i].vPosition.x = vHeightPos.x;
                pVertices[i].vPosition.y = vHeightPos.y;
                pVertices[i].vPosition.z = vHeightPos.z;
            }
        }

        m_pVIBufferCom->UnLock();
    }
}

HRESULT CInstanceModel::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();

    return S_OK;
}

HRESULT CInstanceModel::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

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


CGameObject* CInstanceModel::Clone(void* pArg)
{
    return nullptr;
}

void CInstanceModel::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
}
