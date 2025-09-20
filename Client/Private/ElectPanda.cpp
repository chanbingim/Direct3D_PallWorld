#include "ElectPanda.h"

#include "GameInstance.h"

CElectPanda::CElectPanda(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
    m_PellID = 2;
}

CElectPanda::CElectPanda(const CElectPanda& rhs) :
    CPellBase(rhs)
{
}

HRESULT CElectPanda::Initalize_Prototype()
{


    return S_OK;
} 

HRESULT CElectPanda::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_PartObjects()))
        return E_FAIL;

    return S_OK;
}

void CElectPanda::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CElectPanda::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CElectPanda::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CElectPanda::Render()
{


    return S_OK;
}

HRESULT CElectPanda::ADD_PartObjects()
{
    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricPanda_Body_Default"), TEXT("Part_Body"))))
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
