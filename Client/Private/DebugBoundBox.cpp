#include "DebugBoundBox.h"

#include "GameInstance.h"

CDebugBoundBox::CDebugBoundBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CGameObject(pDevice, pContext)
{
}

CDebugBoundBox::CDebugBoundBox(const CDebugBoundBox& rhs) :
    CGameObject(rhs)
{
}

HRESULT CDebugBoundBox::Initalize_Prototype()
{

    return S_OK;
}

HRESULT CDebugBoundBox::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    return S_OK;
}

void CDebugBoundBox::Priority_Update(_float fDeletaTime)
{
}

void CDebugBoundBox::Update(_float fDeletaTime)
{
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

void CDebugBoundBox::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CDebugBoundBox::Render()
{
    m_pCollision->Render();
    return S_OK;
}

HRESULT CDebugBoundBox::ADD_Components()
{
    CBoxCollision::BOX_COLLISION_DESC BoxColDesc = {};
    BoxColDesc.pOwner = this;
    BoxColDesc.Extents = {1.f, 1.f, 1.f};

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionBox"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &BoxColDesc)))
        return E_FAIL;

    return S_OK;
}

CDebugBoundBox* CDebugBoundBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDebugBoundBox* pDebugBoundBox = new CDebugBoundBox(pDevice, pContext);
    if (FAILED(pDebugBoundBox->Initalize_Prototype()))
    {
        Safe_Release(pDebugBoundBox);
        MSG_BOX("CREATE FAIL : DEUBG BOUND BOX");
    }
    return pDebugBoundBox;
}

CGameObject* CDebugBoundBox::Clone(void* pArg)
{
    CDebugBoundBox* pDebugBoundBox = new CDebugBoundBox(*this);
    if (FAILED(pDebugBoundBox->Initialize(pArg)))
    {
        Safe_Release(pDebugBoundBox);
        MSG_BOX("CLONE FAIL : DEUBG BOUND BOX");
    }
    return pDebugBoundBox;
}

void CDebugBoundBox::Free()
{
    __super::Free();

    Safe_Release(m_pCollision);
}
