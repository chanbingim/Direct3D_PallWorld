#include "PellStateLaunched.h"

#include "GameInstance.h"
#include "PellBase.h"

CPellStateLaunched::CPellStateLaunched(const char* szStateName) :
    CState(szStateName)
{
}

void CPellStateLaunched::OnStateEnter(void* pArg)
{
    PELL_STATE_LAUNCHED_DESC* pStateDesc = static_cast<PELL_STATE_LAUNCHED_DESC*>(pArg);
    m_vDir = pStateDesc->vDir;
    m_fMoveSpeed = pStateDesc->fMoveSpeed;
    m_pActPell = pStateDesc->pActPell;
        
    m_pNavigation = pStateDesc->pNavigation;
    Safe_AddRef(m_pNavigation);
}

void CPellStateLaunched::OnStateExcution(_float fDeletaTime, void* pArg)
{
    m_szStateName = "Carrying";

    _float3 vPellPos = m_pActPell->GetTransform()->GetPosition();
    _vector vCalvPellPos = XMLoadFloat3(&vPellPos);

    _vector vMovement = XMLoadFloat3(&m_vDir) * m_fMoveSpeed * fDeletaTime;
    if (m_pNavigation->IsMove(vCalvPellPos + vMovement))
        m_pActPell->GetTransform()->ADD_Position(vMovement);
}

void CPellStateLaunched::OnStateExit(void* pArg)
{
    Safe_Release(m_pNavigation);
    m_pActPell = nullptr;
}

CPellStateLaunched* CPellStateLaunched::Create(const char* szStateName)
{
    return new CPellStateLaunched(szStateName);
}

void CPellStateLaunched::Free()
{
    __super::Free();

    Safe_Release(m_pNavigation);
}
