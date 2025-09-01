#include "Herorong.h"

#include "GameInstance.h"

CHerorong::CHerorong(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
}

CHerorong::CHerorong(const CHerorong& rhs) :
    CPellBase(rhs)
{
}

HRESULT CHerorong::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CHerorong::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CHerorong::Priority_Update(_float fDeletaTime)
{
}

void CHerorong::Update(_float fDeletaTime)
{
    m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime);
}

void CHerorong::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CHerorong::Render()
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

HRESULT CHerorong::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Herorong_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CHerorong* CHerorong::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CHerorong* pHerorong = new CHerorong(pGraphic_Device, pDeviceContext);
    if (FAILED(pHerorong->Initalize_Prototype()))
    {
        Safe_Release(pHerorong);
        MSG_BOX("CREATE FAIL : HERORONG");
    }
    return pHerorong;
}

CGameObject* CHerorong::Clone(void* pArg)
{
    CHerorong* pHerorong = new CHerorong(*this);
    if (FAILED(pHerorong->Initialize(pArg)))
    {
        Safe_Release(pHerorong);
        MSG_BOX("CLONE FAIL : HERORONG");
    }
    return pHerorong;
}

void CHerorong::Free()
{
    __super::Free();
}
