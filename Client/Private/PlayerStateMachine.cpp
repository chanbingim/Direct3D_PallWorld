#include "PlayerStateMachine.h"

#include "StringHelper.h"
#pragma region Parent State
#include "HitState.h"
#include "CrouchState.h"
#include "ClimbState.h"
#include "JumpState.h"
#include "PlayerIdleState.h"
#pragma endregion

#pragma region Child State
#include "PlayerWalkState.h"
#include "PlayerChildIdleState.h"
#include "PlayerJog.h"
#include "PlayerSprint.h"
#pragma endregion

CPlayerStateMachine::CPlayerStateMachine() :
    CFiniteStateMachine()
{
}

CPlayerStateMachine::CPlayerStateMachine(const CPlayerStateMachine& rhs) :
    CFiniteStateMachine(rhs)
{
}

HRESULT CPlayerStateMachine::Initialize(void* pArg)
{
    m_MaxHeirary = 3;
    m_States = new unordered_map<_wstring, CState*>[m_MaxHeirary];
    m_CurrentStates = new pair<_wstring, CState*>[m_MaxHeirary];

    if (FAILED(ADD_PlayerMoveState(0)))
        return E_FAIL;

    if (FAILED(ADD_PlayerChildState(2)))
        return E_FAIL;

    SettingPlayerState(pArg);
    ChangeState(0, TEXT("Default"));
    ChangeState(2, TEXT("Idle"));
    return S_OK;
}

void CPlayerStateMachine::Update(_float DeltaTime)
{
    for (_uInt i = 0; i < m_MaxHeirary; ++i)
    {
        if(m_CurrentStates[i].second)
            m_CurrentStates[i].second->PlayState();
    }
        
}

void CPlayerStateMachine::ChangeState(_uInt iIndex, const _wstring& StateTag)
{
    auto pair = m_States[iIndex].find(StateTag);
    if (pair == m_States[iIndex].end())
        return;

    if (m_CurrentStates[iIndex].second != pair->second)
    {
        if (m_CurrentStates[iIndex].second)
            m_CurrentStates[iIndex].second->OnEndState();

        pair->second->OnEnterState();
        m_CurrentStates[iIndex].first = pair->first;
        m_CurrentStates[iIndex].second = pair->second;
    }
}

_string CPlayerStateMachine::GetStateFullName()
{
    _wstring FullName = {};

    if (lstrcmp(m_CurrentStates[0].first.c_str(), L"Default"))
        FullName = m_CurrentStates[0].first + L"_";

    FullName += m_CurrentStates[2].first;
    if (m_StateData.bIsAiming)
        FullName += L"_Anim";

    //무기 타입에 의해 네이밍
    switch (m_StateData.eWeaponType)
    {
    case WEAPON::NONE :
        FullName += L"_None";
        break;
    }
    CStringHelper::ConvertWideToUTF(FullName.c_str(), m_FullName);
    return m_FullName;
}

HRESULT CPlayerStateMachine::ADD_PlayerMoveState(_uInt iIndex)
{
    if (FAILED(AddState(iIndex, TEXT("Hit"), CHitState::Create())))
        return E_FAIL;

    if (FAILED(AddState(iIndex, TEXT("Crouch"), CCrouchState::Create())))
        return E_FAIL;

    if (FAILED(AddState(iIndex, TEXT("Climb"), CClimbState::Create())))
        return E_FAIL;

    if (FAILED(AddState(iIndex, TEXT("Default"), CPlayerIdleState::Create())))
        return E_FAIL;

    if (FAILED(AddState(iIndex, TEXT("Jump"), CJumpState::Create())))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerStateMachine::ADD_PlayerChildState(_uInt iIndex)
{
    if (FAILED(AddState(iIndex, TEXT("Walk"), CPlayerWalkState::Create())))
        return E_FAIL;

    if (FAILED(AddState(iIndex, TEXT("Jog"), CPlayerJogState::Create())))
        return E_FAIL;

    if (FAILED(AddState(iIndex, TEXT("Sprint"), CPlayerSprintState::Create())))
        return E_FAIL;

    if (FAILED(AddState(iIndex, TEXT("Idle"), CPlayerChildIdleState::Create())))
        return E_FAIL;

    return S_OK;
}

void CPlayerStateMachine::SettingPlayerState(void* pArg)
{
    if (nullptr == pArg)
    {
        //플레이어가 데이터를 외부에서 받아오지 않았을경우
        //초기 데이터 세팅을 위해 데이터를 세팅해준다.
        m_StateData.bIsAiming = false;
        m_StateData.bIsAttacking = false;

        m_StateData.eWeaponType = WEAPON::NONE;
        m_StateData.eMove_State = MOVE_ACTION::DEFAULT;
        m_StateData.eMove_Child_State = MOVE_CHILD_ACTION::IDLE;
        m_StateData.eNone_Move_State = NONE_MOVE_ACTION::END;
        return;
    }

    PLAYER_STATE* Desc = static_cast<PLAYER_STATE *>(pArg);
    if (Desc)
    {

    }
}

CPlayerStateMachine* CPlayerStateMachine::Create()
{
    return new CPlayerStateMachine();
}

void CPlayerStateMachine::Free()
{
    __super::Free();

    Safe_Delete_Array(m_CurrentStates);
    Safe_Delete_Array(m_States);
}
