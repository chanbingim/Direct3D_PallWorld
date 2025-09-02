#include "HedgehogBody.h"

#include "GameInstance.h"
CHedgehogBody::CHedgehogBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPellBody(pDevice, pContext)
{
}

CHedgehogBody::CHedgehogBody(const CHedgehogBody& rhs) :
    CPellBody(rhs)
{
}

HRESULT CHedgehogBody::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CHedgehogBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CHedgehogBody::Priority_Update(_float fDeletaTime)
{
}

void CHedgehogBody::Update(_float fDeletaTime)
{
    m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime);
}

void CHedgehogBody::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CHedgehogBody::Render()
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

HRESULT CHedgehogBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Hedgehog_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CHedgehogBody* CHedgehogBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHedgehogBody* pHedgehoBody = new CHedgehogBody(pDevice, pContext);
    if (FAILED(pHedgehoBody->Initalize_Prototype()))
    {
        Safe_Release(pHedgehoBody);
        MSG_BOX("CREATE FAIL : HEDGEHO Body");
    }
    return pHedgehoBody;
}

CGameObject* CHedgehogBody::Clone(void* pArg)
{
    CHedgehogBody* pHedgehoBody = new CHedgehogBody(*this);
    if (FAILED(pHedgehoBody->Initialize(pArg)))
    {
        Safe_Release(pHedgehoBody);
        MSG_BOX("CLONE FAIL : HEDGEHO Body");
    }
    return pHedgehoBody;
}

void CHedgehogBody::Free()
{
    __super::Free();
}
