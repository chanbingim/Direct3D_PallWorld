#include "BedCat.h"

#include "GameInstance.h"

CBedCat::CBedCat(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
}

CBedCat::CBedCat(const CBedCat& rhs) :
    CPellBase(rhs)
{
}

HRESULT CBedCat::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBedCat::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CBedCat::Priority_Update(_float fDeletaTime)
{
}

void CBedCat::Update(_float fDeletaTime)
{
    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_F3))
    {
        m_pVIBufferCom->Export("../Bin/Save/ModelData/BedCat/BedCat.dat");
    }
}

void CBedCat::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBedCat::Render()
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

HRESULT CBedCat::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_BedCat_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CBedCat* CBedCat::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CBedCat* pBedCat = new CBedCat(pGraphic_Device, pDeviceContext);
    if (FAILED(pBedCat->Initalize_Prototype()))
    {
        Safe_Release(pBedCat);
        MSG_BOX("CREATE FAIL : BED CAT");
    }
    return pBedCat;
}

CGameObject* CBedCat::Clone(void* pArg)
{
    CBedCat* pBedCat = new CBedCat(*this);
    if (FAILED(pBedCat->Initialize(pArg)))
    {
        Safe_Release(pBedCat);
        MSG_BOX("CLONE FAIL : BED CAT");
    }
    return pBedCat;
}

void CBedCat::Free()
{
    __super::Free();
}
