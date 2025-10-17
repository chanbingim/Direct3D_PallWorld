#include "PellCarryState.h"

#include "GameInstance.h"

#include "PlayerManager.h"
#include "Player.h"

#include "PellBase.h"
#include "PellBody.h"

CPellCarryState::CPellCarryState(const char* szStateName) :
    CState(szStateName),
    m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

void CPellCarryState::OnStateEnter(void* pArg)
{
    PELL_CARRY_DESC* pCarryDesc = static_cast<PELL_CARRY_DESC*>(pArg);
    m_pActPell = pCarryDesc->pActPell;
    m_pSoketMat = pCarryDesc->pSoketMat;
    
    m_pPellCollision = pCarryDesc->pPellCollision;
    Safe_AddRef(m_pPellCollision);

    m_iPhaseIndex = 0;
}

void CPellCarryState::OnStateExcution(_float fDeletaTime, void* pArg)
{
    UpdateStateState();
    NextPhaseAnimation();

    switch (m_iPhaseIndex)
    {
    case 0:
        m_szStateName = "Carrying_Start";
        m_bStateAnimLoop = false;
        break;
    case 1:
        m_szStateName = "Carrying";
        m_bStateAnimLoop = true;
        break;
    }
}

void CPellCarryState::OnStateExit(void* pArg)
{
    m_pActPell->AttachSocket(nullptr, 0b00000011);
    m_pActPell = nullptr;
    m_pSoketMat = nullptr;

    Safe_Release(m_pPellCollision);
}

void CPellCarryState::UpdateStateState()
{
    auto pPlayer = CPlayerManager::GetInstance()->GetCurrentPlayer();

    if (nullptr != pPlayer)
    {
        _float3 vScale = m_pActPell->GetTransform()->GetScale();
        _matrix pPlayerWorldMat = XMLoadFloat4x4(&pPlayer->GetTransform()->GetWorldMat());

        pPlayerWorldMat.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
        _matrix pSocketMat = XMLoadFloat4x4(m_pSoketMat) * pPlayerWorldMat;

        XMStoreFloat4x4(&m_CombinedMat, pSocketMat);
        m_pActPell->AttachSocket(&m_CombinedMat, 0b00000001);

        m_pActPell->GetPellBody()->GetTransform()->SetRotation(_float3(XMConvertToRadians(90.f), 0.f, XMConvertToRadians(180.f)));
        m_pActPell->GetTransform()->SetPosition(pPlayer->GetTransform()->GetPosition());
    }
}

void CPellCarryState::NextPhaseAnimation()
{
    if (false == m_bStateAnimLoop && m_pActPell->GetFinisehdAnimation())
    {
        m_iPhaseIndex++;
    }
}

CPellCarryState* CPellCarryState::Create(const char* szStateName)
{
    return new CPellCarryState(szStateName);
}

void CPellCarryState::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pPellCollision);
}
