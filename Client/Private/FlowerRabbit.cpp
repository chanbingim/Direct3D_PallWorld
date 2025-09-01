#include "FlowerRabbit.h"

#include "GameInstance.h"

CFlowerRabbit::CFlowerRabbit(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
}

CFlowerRabbit::CFlowerRabbit(const CFlowerRabbit& rhs) :
    CPellBase(rhs)
{
}

HRESULT CFlowerRabbit::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CFlowerRabbit::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CFlowerRabbit::Priority_Update(_float fDeletaTime)
{
}

void CFlowerRabbit::Update(_float fDeletaTime)
{
    m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime);
}

void CFlowerRabbit::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CFlowerRabbit::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(0);

        m_pVIBufferCom->Render(i);
    }

    return S_OK;
}

HRESULT CFlowerRabbit::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_FlowerRabbit_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CFlowerRabbit* CFlowerRabbit::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CFlowerRabbit* pFlowerRabbit = new CFlowerRabbit(pGraphic_Device, pDeviceContext);
    if (FAILED(pFlowerRabbit->Initalize_Prototype()))
    {
        Safe_Release(pFlowerRabbit);
        MSG_BOX("CREATE FAIL : Flower Rabbit");
    }
    return pFlowerRabbit;
}

CGameObject* CFlowerRabbit::Clone(void* pArg)
{
    CFlowerRabbit* pFlowerRabbit = new CFlowerRabbit(*this);
    if (FAILED(pFlowerRabbit->Initialize(pArg)))
    {
        Safe_Release(pFlowerRabbit);
        MSG_BOX("CLONE FAIL : Flower Rabbit");
    }
    return pFlowerRabbit;
}

void CFlowerRabbit::Free()
{
    __super::Free();
}
