#include "FrogBody.h"

#include "GameInstance.h"
CFrogBody::CFrogBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPellBody(pDevice, pContext)
{
}

CFrogBody::CFrogBody(const CFrogBody& rhs) :
    CPellBody(rhs)
{
}

HRESULT CFrogBody::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CFrogBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CFrogBody::Priority_Update(_float fDeletaTime)
{
}

void CFrogBody::Update(_float fDeletaTime)
{
    m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime);
}

void CFrogBody::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CFrogBody::Render()
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

HRESULT CFrogBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Frog_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CFrogBody* CFrogBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFrogBody* pFrog = new CFrogBody(pDevice, pContext);
    if (FAILED(pFrog->Initalize_Prototype()))
    {
        Safe_Release(pFrog);
        MSG_BOX("CREATE FAIL : Frog Body");
    }
    return pFrog;
}

CGameObject* CFrogBody::Clone(void* pArg)
{
    CFrogBody* pFrog = new CFrogBody(*this);
    if (FAILED(pFrog->Initialize(pArg)))
    {
        Safe_Release(pFrog);
        MSG_BOX("CREATE FAIL : Frog Body");
    }
    return pFrog;
}

void CFrogBody::Free()
{
    __super::Free();
}
