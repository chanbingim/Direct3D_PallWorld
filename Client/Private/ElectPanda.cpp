#include "ElectPanda.h"

#include "GameInstance.h"

CElectPanda::CElectPanda(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
}

CElectPanda::CElectPanda(const CElectPanda& rhs) :
    CPellBase(rhs)
{
}

HRESULT CElectPanda::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CElectPanda::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CElectPanda::Priority_Update(_float fDeletaTime)
{
}

void CElectPanda::Update(_float fDeletaTime)
{
}

void CElectPanda::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CElectPanda::Render()
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

HRESULT CElectPanda::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_ElectricPanda_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CElectPanda* CElectPanda::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CElectPanda* pElectPanda = new CElectPanda(pGraphic_Device, pDeviceContext);
    if (FAILED(pElectPanda->Initalize_Prototype()))
    {
        Safe_Release(pElectPanda);
        MSG_BOX("CREATE FAIL : ElECT PANDA");
    }
    return pElectPanda;
}

CGameObject* CElectPanda::Clone(void* pArg)
{
    CElectPanda* pElectPanda = new CElectPanda(*this);
    if (FAILED(pElectPanda->Initialize(pArg)))
    {
        Safe_Release(pElectPanda);
        MSG_BOX("CLONE FAIL : ElECT PANDA");
    }
    return pElectPanda;
}

void CElectPanda::Free()
{
    __super::Free();
}
