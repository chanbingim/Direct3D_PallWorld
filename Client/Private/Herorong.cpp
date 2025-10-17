#include "Herorong.h"

#include "GameInstance.h"

CHerorong::CHerorong(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
    m_PellID = 6;
}

CHerorong::CHerorong(const CHerorong& rhs) :
    CPellBase(rhs)
{
}

HRESULT CHerorong::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CHerorong::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_PartObjects()))
        return E_FAIL;

    return S_OK;
}

void CHerorong::Priority_Update(_float fDeletaTime)
{
}

void CHerorong::Update(_float fDeletaTime)
{
   
}

void CHerorong::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CHerorong::Render()
{
   

    return S_OK;
}

HRESULT CHerorong::ADD_PartObjects()
{
    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricPanda_Body_Default"), TEXT("Part_Body"))))
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
