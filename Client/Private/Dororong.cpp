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

    if (FAILED(ADD_PartObjects()))
        return E_FAIL;

    return S_OK;
}

void CDororong::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CDororong::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CDororong::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CDororong::Render()
{
    

    return S_OK;
}

HRESULT CDororong::ADD_PartObjects()
{
    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Drorong_Body_Default"), TEXT("Part_Body"))))
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
