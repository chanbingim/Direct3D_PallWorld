#include "Yeti.h"

#include "GameInstance.h"

CYeti::CYeti(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CPellBase(pGraphic_Device, pDeviceContext)
{
    m_PellID = 7;
}

CYeti::CYeti(const CYeti& rhs) :
    CPellBase(rhs)
{
}

HRESULT CYeti::Initalize_Prototype()
{

    return S_OK;
}

HRESULT CYeti::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_PartObjects()))
        return E_FAIL;

    return S_OK;
}

void CYeti::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CYeti::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CYeti::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CYeti::Render()
{
  

    return S_OK;
}

HRESULT CYeti::ADD_PartObjects()
{
    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Yeti_Body_Default"), TEXT("Part_Body"))))
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
