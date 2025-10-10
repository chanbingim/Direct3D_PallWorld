#include "Npc.h"

#include "GameInstance.h"
#include "PellManager.h"

#pragma region PARTS HEADER
#include "NpcStateMachine.h"
#include "NpcBody.h"
#pragma endregion

CNpc::CNpc(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CContainerObject(pGraphic_Device, pDeviceContext)
{
}

CNpc::CNpc(const CNpc& rhs) :
    CContainerObject(rhs),
    m_NpcID(rhs.m_NpcID)
{
}

HRESULT CNpc::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CNpc::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(SetUpDefaultNpcData()))
        return E_FAIL;

    return S_OK;
}

void CNpc::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CNpc::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CNpc::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);

    if (!m_IsDead)
        m_pGameInstance->ADD_CollisionList(m_pCollision);
}

HRESULT CNpc::Render()
{
    if (!m_IsDead)
    {
        m_pCollision->Render();
    }

    return S_OK;
}

const _bool CNpc::GetFinisehdAnimation() const
{
    if (nullptr == m_pNpcBody)
        return false;
    return m_pNpcBody->FinishedAnimation();
}

HRESULT CNpc::SetUpDefaultNpcData()
{
    return S_OK;
}

HRESULT CNpc::ADD_PellInfoUI()
{
    return S_OK;
}

_bool CNpc::NpcPlayFSM(_float fDeletaTime)
{
    m_pNpcFsm->Update(fDeletaTime);
    return m_pNpcFsm->GetLayerAnimLoop(TEXT("Dialog_Layer"));
}

void CNpc::ShowActionUI()
{
}

CGameObject* CNpc::Clone(void* pArg)
{
    return nullptr;
}

void CNpc::Free()
{
    __super::Free();

    Safe_Release(m_pNpcFsm);
    Safe_Release(m_pNevigation);
    Safe_Release(m_pCollision);
}
