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

    RecomputeInstanceModelHeight();
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
