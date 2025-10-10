#include "FemalePeople_1.h"

#include "GameInstance.h"
#include "NpcStateMachine.h"
#include "NpcBody.h"

CFemalePeople_1::CFemalePeople_1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CNpc(pDevice, pContext)
{
}

CFemalePeople_1::CFemalePeople_1(const CFemalePeople_1& rhs) :
    CNpc(rhs)
{
}

HRESULT CFemalePeople_1::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CFemalePeople_1::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_NpcFsm()))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_PartObjects()))
        return E_FAIL;

    m_eTeam = NPC_TEAM::FRENDLY;
    return S_OK;
}

void CFemalePeople_1::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
    NpcPlayFSM(fDeletaTime);

    m_pNevigation->ComputeHeight(m_pTransformCom, false);
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

void CFemalePeople_1::Update(_float fDeletaTime)
{
    m_pNpcBody->PlayAnimation(m_pNpcFsm->GetStateFullName().c_str(), m_bIsLoop);
    __super::Update(fDeletaTime);
}

void CFemalePeople_1::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        __super::Late_Update(fDeletaTime);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CFemalePeople_1::Render()
{
    __super::Render();
    return S_OK;
}

void CFemalePeople_1::OverlapEvent(_float3 vDir, CGameObject* pHitActor)
{
}

HRESULT CFemalePeople_1::ADD_Components()
{
    CBoxCollision::BOX_COLLISION_DESC BoxDesc = {};
    BoxDesc.pOwner = this;
    BoxDesc.Extents = _float3(0.5f, 1.2f, 0.5f);;
    BoxDesc.vCneter = _float3(0.f, BoxDesc.Extents.y * 0.5f, 0.f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionBox"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &BoxDesc)))
        return E_FAIL;
    m_pCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { OverlapEvent(vDir, pHitActor); });

    auto Object = m_pGameInstance->GetAllObejctToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Terrian"))->begin();
    auto OriginNav = static_cast<CNavigation*>((*Object)->Find_Component(TEXT("NaviMesh_Com")));
    CNavigation::NAVIGATION_DESC Desc = {};
    _float3 vPos = m_pTransformCom->GetPosition();
    Desc.iCurrentCellIndex = 100;

    m_pTransformCom->SetPosition(OriginNav->CellCenterPos(Desc.iCurrentCellIndex));
    m_pNevigation = static_cast<CNavigation*>(OriginNav->Clone(&Desc));

    Safe_AddRef(m_pNevigation);
    m_pComponentMap.emplace(TEXT("NaviMesh_Com"), m_pNevigation);
    return S_OK;
}

HRESULT CFemalePeople_1::ADD_PartObjects()
{
    CNpcBody::PARTOBJECT_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };

    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Female_People_Body_1"), TEXT("Part_Body"), &Desc)))
        return E_FAIL;

    m_pNpcBody = static_cast<CNpcBody*>(FindPartObject(TEXT("Part_Body")));
    return S_OK;
}

HRESULT CFemalePeople_1::Setup_NpcFsm()
{
    CNpcStateMachine::FSM_DESC FSMDesc = {};
    FSMDesc.iLayerSize = 2;
    FSMDesc.pOwner = this;

    m_pNpcFsm = CNpcStateMachine::Create();
    if (FAILED(m_pNpcFsm->Initialize(&FSMDesc)))
        return E_FAIL;
    return S_OK;
}

CFemalePeople_1* CFemalePeople_1::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CFemalePeople_1* pFemalePeople = new CFemalePeople_1(pGraphic_Device, pDeviceContext);
    if (FAILED(pFemalePeople->Initalize_Prototype()))
    {
        Safe_Release(pFemalePeople);
        MSG_BOX("CREATE FAIL : FEMALE PEOPLE 1");
    }
    return pFemalePeople;
}

CGameObject* CFemalePeople_1::Clone(void* pArg)
{
    CFemalePeople_1* pFemalePeople = new CFemalePeople_1(*this);
    if (FAILED(pFemalePeople->Initialize(pArg)))
    {
        Safe_Release(pFemalePeople);
        MSG_BOX("CLONE FAIL : FEMALE PEOPLE 1");
    }
    return pFemalePeople;
}

void CFemalePeople_1::Free()
{
    __super::Free();
}
