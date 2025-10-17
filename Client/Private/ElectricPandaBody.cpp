#include "ElectricPandaBody.h"

#include "GameInstance.h"
CElectricPandaBody::CElectricPandaBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPellBody(pDevice, pContext)
{
}

CElectricPandaBody::CElectricPandaBody(const CElectricPandaBody& rhs) :
    CPellBody(rhs)
{
}

HRESULT CElectricPandaBody::Initalize_Prototype()
{
  

    return S_OK;
}

HRESULT CElectricPandaBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CElectricPandaBody::Priority_Update(_float fDeletaTime)
{
}

void CElectricPandaBody::Update(_float fDeletaTime)
{
    m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime);
}

void CElectricPandaBody::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CElectricPandaBody::Render()
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

HRESULT CElectricPandaBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_ElectricPanda_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CElectricPandaBody* CElectricPandaBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CElectricPandaBody* pElectPandaBody = new CElectricPandaBody(pDevice, pContext);
    if (FAILED(pElectPandaBody->Initalize_Prototype()))
    {
        Safe_Release(pElectPandaBody);
        MSG_BOX("CREATE FAIL : ElECT PANDA BODY");
    }
    return pElectPandaBody;
}

CGameObject* CElectricPandaBody::Clone(void* pArg)
{
    CElectricPandaBody* pElectPandaBody = new CElectricPandaBody(*this);
    if (FAILED(pElectPandaBody->Initialize(pArg)))
    {
        Safe_Release(pElectPandaBody);
        MSG_BOX("CLONE FAIL : ElECT PANDA BODY");
    }
    return pElectPandaBody;
}

void CElectricPandaBody::Free()
{
    __super::Free();
}
