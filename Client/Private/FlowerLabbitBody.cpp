#include "FlowerLabbitBody.h"

#include "GameInstance.h"
CFlowerLabbitBody::CFlowerLabbitBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPellBody(pDevice, pContext)
{
}

CFlowerLabbitBody::CFlowerLabbitBody(const CFlowerLabbitBody& rhs) :
    CPellBody(rhs)
{
}

HRESULT CFlowerLabbitBody::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CFlowerLabbitBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CFlowerLabbitBody::Priority_Update(_float fDeletaTime)
{
}

void CFlowerLabbitBody::Update(_float fDeletaTime)
{
    m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime);
}

void CFlowerLabbitBody::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CFlowerLabbitBody::Render()
{
    return E_NOTIMPL; _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(0);

        m_pVIBufferCom->Render(i);
    }

    return S_OK;
}

HRESULT CFlowerLabbitBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_FlowerRabbit_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CFlowerLabbitBody* CFlowerLabbitBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFlowerLabbitBody* pFlowerRabbitBody = new CFlowerLabbitBody(pDevice, pContext);
    if (FAILED(pFlowerRabbitBody->Initalize_Prototype()))
    {
        Safe_Release(pFlowerRabbitBody);
        MSG_BOX("CREATE FAIL : Flower Rabbit Body");
    }
    return pFlowerRabbitBody;
}

CGameObject* CFlowerLabbitBody::Clone(void* pArg)
{
    CFlowerLabbitBody* pFlowerRabbitBody = new CFlowerLabbitBody(*this);
    if (FAILED(pFlowerRabbitBody->Initialize(pArg)))
    {
        Safe_Release(pFlowerRabbitBody);
        MSG_BOX("CLONE FAIL : Flower Rabbit Body");
    }
    return pFlowerRabbitBody;
}

void CFlowerLabbitBody::Free()
{
    __super::Free();
}
