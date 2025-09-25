#include "Flower.h"

#include "GameInstance.h"
#include "TerrainManager.h"

CFlower::CFlower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CInstanceModel(pDevice, pContext)
{
}

CFlower::CFlower(const CFlower& rhs) :
    CInstanceModel(rhs)
{
}

HRESULT CFlower::Initalize_Prototype()
{

    return S_OK;
}

HRESULT CFlower::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    auto pNaviMesh = CTerrainManager::GetInstance()->GetNavimesh();

    D3D11_MAPPED_SUBRESOURCE SubResource = {};

    _float3 vPos = m_pTransformCom->GetPosition();
    _uInt iNumInstance = m_pVIBufferCom->GetNumInstance();
    m_pVIBufferCom->Lock(&SubResource);;
    VTX_INSTANCE_DEFAULT_DESC* pVertices = static_cast<VTX_INSTANCE_DEFAULT_DESC*>(SubResource.pData);
    if (pVertices)
    {
        for (_uInt i = 0; i < iNumInstance; i++)
        {
         
            _float3 vHeightPos = { pVertices[i].vPosition.x + vPos.x,
                                   pVertices[i].vPosition.y + vPos.y,
                                   pVertices[i].vPosition.z + vPos.z };
            pNaviMesh->ComputeHeight(&vHeightPos);
            pVertices[i].vPosition.x = vHeightPos.x;
            pVertices[i].vPosition.y = vHeightPos.y;
            pVertices[i].vPosition.z = vHeightPos.z;
        }
    }

    m_pVIBufferCom->UnLock();

    return S_OK;
}

void CFlower::Priority_Update(_float fDeletaTime)
{
}

void CFlower::Update(_float fDeletaTime)
{
}

void CFlower::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CFlower::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetModelIndex();

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        __super::Apply_ConstantShaderResources(i);
        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render_VIBuffer(i);
    }

    return S_OK;
}

HRESULT CFlower::ADD_Components()
{
    // ¸ðµ¨ Á¤º¸
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Instance_Flower"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_InstanceMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CFlower* CFlower::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFlower* pFlower = new CFlower(pDevice, pContext);
    if (FAILED(pFlower->Initalize_Prototype()))
    {
        Safe_Release(pFlower);
        MSG_BOX("CREATE FAIL : Flower");
    }
    return pFlower;
}

CGameObject* CFlower::Clone(void* pArg)
{
    CGameObject* pFlower = new CFlower(*this);
    if (FAILED(pFlower->Initialize(pArg)))
    {
        Safe_Release(pFlower);
        MSG_BOX("CLONE FAIL : Flower");
    }

    return pFlower;
}

void CFlower::Free()
{
    __super::Free();
}
