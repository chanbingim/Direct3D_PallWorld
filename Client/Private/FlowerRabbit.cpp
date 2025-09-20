#include "FlowerRabbit.h"

#include "GameInstance.h"

CFlowerRabbit::CFlowerRabbit(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
    m_PellID = 3;
}

CFlowerRabbit::CFlowerRabbit(const CFlowerRabbit& rhs) :
    CPellBase(rhs)
{
}

HRESULT CFlowerRabbit::Initalize_Prototype()
{

    return S_OK;
}

HRESULT CFlowerRabbit::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    return S_OK;
}

void CFlowerRabbit::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CFlowerRabbit::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CFlowerRabbit::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CFlowerRabbit::Render()
{
    

    return S_OK;
}

HRESULT CFlowerRabbit::ADD_Components()
{
    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_FlowerRabbit_Body_Default"), TEXT("Part_Body"))))
        return E_FAIL;

    return S_OK;
}

CFlowerRabbit* CFlowerRabbit::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CFlowerRabbit* pFlowerRabbit = new CFlowerRabbit(pGraphic_Device, pDeviceContext);
    if (FAILED(pFlowerRabbit->Initalize_Prototype()))
    {
        Safe_Release(pFlowerRabbit);
        MSG_BOX("CREATE FAIL : Flower Rabbit");
    }
    return pFlowerRabbit;
}

CGameObject* CFlowerRabbit::Clone(void* pArg)
{
    CFlowerRabbit* pFlowerRabbit = new CFlowerRabbit(*this);
    if (FAILED(pFlowerRabbit->Initialize(pArg)))
    {
        Safe_Release(pFlowerRabbit);
        MSG_BOX("CLONE FAIL : Flower Rabbit");
    }
    return pFlowerRabbit;
}

void CFlowerRabbit::Free()
{
    __super::Free();
}
