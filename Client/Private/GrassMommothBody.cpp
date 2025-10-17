#include "GrassMommothBody.h"

#include "GameInstance.h"

CGrassMommothBody::CGrassMommothBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPellBody(pDevice, pContext)
{
}

CGrassMommothBody::CGrassMommothBody(const CGrassMommothBody& rhs) :
    CPellBody(rhs)
{
}

HRESULT CGrassMommothBody::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CGrassMommothBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CGrassMommothBody::Priority_Update(_float fDeletaTime)
{
}

void CGrassMommothBody::Update(_float fDeletaTime)
{
    m_bIsAnimFinished = m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime, 10.f, m_bIsAnimLoop);
}

void CGrassMommothBody::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CGrassMommothBody::Render()
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

HRESULT CGrassMommothBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_GrassMommoth_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CGrassMommothBody* CGrassMommothBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGrassMommothBody* pGrassMommothBody = new CGrassMommothBody(pDevice, pContext);
    if (FAILED(pGrassMommothBody->Initalize_Prototype()))
    {
        Safe_Release(pGrassMommothBody);
        MSG_BOX("CREATE FAIL : Grass Mommoth BODY");
    }
    return pGrassMommothBody;
}

CGameObject* CGrassMommothBody::Clone(void* pArg)
{
    CGrassMommothBody* pGrassMommothBody = new CGrassMommothBody(*this);
    if (FAILED(pGrassMommothBody->Initialize(pArg)))
    {
        Safe_Release(pGrassMommothBody);
        MSG_BOX("CLONE FAIL : Grass Mommoth BODY");
    }
    return pGrassMommothBody;
}

void CGrassMommothBody::Free()
{
    __super::Free();
}
