#include "BedCat.h"

#include "GameInstance.h"

CBedCat::CBedCat(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
    m_PellID = 1;
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

    if (FAILED(ADD_PartObjects()))
        return E_FAIL;

    return S_OK;
}

void CBedCat::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CBedCat::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CBedCat::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CBedCat::Render()
{
   
    return S_OK;
}

HRESULT CBedCat::ADD_PartObjects()
{
    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BedCat_Body_Default"), TEXT("Part_Body"))))
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
