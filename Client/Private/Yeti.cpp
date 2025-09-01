#include "Yeti.h"

#include "GameInstance.h"

CYeti::CYeti(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
}

CYeti::CYeti(const CYeti& rhs) :
    CPellBase(rhs)
{
}

HRESULT CYeti::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CYeti::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CYeti::Priority_Update(_float fDeletaTime)
{
}

void CYeti::Update(_float fDeletaTime)
{
    m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime);
}

void CYeti::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CYeti::Render()
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

HRESULT CYeti::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Yeti_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CYeti* CYeti::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CYeti* pYeti = new CYeti(pGraphic_Device, pDeviceContext);
    if (FAILED(pYeti->Initalize_Prototype()))
    {
        Safe_Release(pYeti);
        MSG_BOX("CREATE FAIL : Yeti");
    }
    return pYeti;
}

CGameObject* CYeti::Clone(void* pArg)
{
    CYeti* pYeti = new CYeti(*this);
    if (FAILED(pYeti->Initialize(pArg)))
    {
        Safe_Release(pYeti);
        MSG_BOX("CLONE FAIL : Yeti");
    }
    return pYeti;
}

void CYeti::Free()
{
    __super::Free();
}
