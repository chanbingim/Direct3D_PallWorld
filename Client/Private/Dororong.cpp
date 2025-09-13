#include "Dororong.h"

#include "GameInstance.h"

#include "TerrainManager.h"
#include "PellStateMachine.h"
#include "Recovery.h"
#include "PellBody.h"

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


    return S_OK;
}

HRESULT CDororong::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_PellFsm()))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_PartObjects()))
        return E_FAIL;

    m_eTeam = PELL_TEAM::NEUTRAL;
    m_PellInfo.CurStemina = m_PellInfo.MaxStemina = 100.f;
    m_fActionTime = 1.f;
    
    return S_OK;
}

void CDororong::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
    PellPlayFSM(fDeletaTime);


   /* if (m_pRecovery->GetRecovering())
        m_PellInfo.CurStemina += m_pRecovery->Update(fDeletaTime);
    else
        m_PellInfo.CurStemina -= 0.1f;*/

    const CPellStateMachine::PELL_STATE& State = m_pPellFsm->GetState();
    if (CPellStateMachine::MOVE_ACTION::PATROL == State.eMove_State)
    {
        _vector vTarget = XMLoadFloat3(&m_vTargetPoint);
        if (!XMVector3Equal(vTarget, XMVectorZero()))
        {
            _float3 vCurPos = m_pTransformCom->GetPosition();
            _vector vPos = XMLoadFloat3(&vCurPos);
            _vector vDir = XMVector3Normalize(vTarget - vPos);

            _vector vMovePos = vDir * 5.f * 0.01f;
            m_pTransformCom->LerpTurn(m_pTransformCom->GetUpVector(), vPos + vMovePos, 5.f, fDeletaTime);
            if (m_pNevigation->IsMove(vPos + vMovePos))
                m_pTransformCom->ADD_Position(vMovePos);
        }
    }

    m_pNevigation->ComputeHeight(m_pTransformCom);
}

void CDororong::Update(_float fDeletaTime)
{
    m_pPellBody->PellPlayAnimation(m_pPellFsm->GetAnimationName().c_str(), m_bIsLoop);
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

HRESULT CDororong::ADD_Components()
{
    CRecovery::RECOVERY_DESC RecoverDesc;
    RecoverDesc.pOwner = this;
    RecoverDesc.fRecoveryTime = 1.0f;
    RecoverDesc.fPerTickRecovery = 10.f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("GamePlay_Component_Recovery"), TEXT("Recovery_Com"), (CComponent**)&m_pRecovery, &RecoverDesc)))
        return E_FAIL;

    _float3 vPos = m_pTransformCom->GetPosition();
    _Int iSapwnCell = m_pTerrainManager->GetCrrentSapwnCell(vPos);

    if (FAILED(m_pTerrainManager->GetCurrentTerrainNaviMesh(vPos, iSapwnCell, (CComponent**)&m_pNevigation)))
        return E_FAIL;
    m_pComponentMap.emplace(TEXT("NaviMesh_Com"), m_pNevigation);

    return S_OK;
}

HRESULT CDororong::ADD_PartObjects()
{
    CPellBody::PARTOBJECT_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };

    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Drorong_Body_Default"), TEXT("Part_Body"), &Desc)))
        return E_FAIL;

    m_pPellBody = static_cast<CPellBody *>(FindPartObject(TEXT("Part_Body")));
    return S_OK;
}

HRESULT CDororong::Setup_PellFsm()
{
    CPellStateMachine::PELLFSM_DESC FSMDesc = {};
    FSMDesc.iLayerSize = 2;
    FSMDesc.pOwner = this;

    m_pPellFsm = CPellStateMachine::Create();
    if (FAILED(m_pPellFsm->Initialize(&FSMDesc)))
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
