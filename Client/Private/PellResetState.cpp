#include "PellResetState.h"
#include "GameInstance.h"

CPellResetState::CPellResetState(const char* szStateName) :
    CState(szStateName)
{
}

void CPellResetState::OnStateEnter(void* pArg)
{
    m_iRandomRestMotion = static_cast<_uInt>(CGameInstance::GetInstance()->Random(0.f, 2.f));
    switch (m_iRandomRestMotion)
    {
    case 0 :
        m_szStateName = "Rest01";
        break;
    case 1:
        m_szStateName = "Rest02";
        break;
    case 2:
        m_szStateName = "Rest03";
        break;
    }

}

void CPellResetState::OnStateExcution(_float fDeletaTime, void* pArg)
{
}

void CPellResetState::OnStateExit(void* pArg)
{
}

CPellResetState* CPellResetState::Create(const char* szStateName)
{
    return new CPellResetState(szStateName);
}

void CPellResetState::Free()
{
    __super::Free();
}
