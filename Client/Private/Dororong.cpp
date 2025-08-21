#include "Dororong.h"

#include "GameInstance.h"

CDororong::CDororong(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
}

CDororong::CDororong(const CDororong& rhs) :
    CPellBase(rhs)
{
}

HRESULT CDororong::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDororong::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CDororong::Priority_Update(_float fDeletaTime)
{
}

void CDororong::Update(_float fDeletaTime)
{
}

void CDororong::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CDororong::Render()
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

HRESULT CDororong::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Drorong_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CDororong* CDororong::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CDororong* pDororong = new CDororong(pGraphic_Device, pDeviceContext);
    if (FAILED(pDororong->Initalize_Prototype()))
    {
        Safe_Release(pDororong);
        MSG_BOX("CREATE FAIL : DRORONG");
    }
    return pDororong;
}

CGameObject* CDororong::Clone(void* pArg)
{
    CDororong* pDororong = new CDororong(*this);
    if (FAILED(pDororong->Initialize(pArg)))
    {
        Safe_Release(pDororong);
        MSG_BOX("CLONE FAIL : DRORONG");
    }
    return pDororong;
}

void CDororong::Free()
{
    __super::Free();
}
