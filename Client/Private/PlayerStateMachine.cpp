#include "PlayerStateMachine.h"

#include "StringHelper.h"

#include "PlayerLowerLayer.h"
#include "PlayerUpperLayer.h"

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
    __super::Initialize(pArg);

    if (FAILED(ADD_PlayerLayer()))
        return E_FAIL;

    SettingPlayerState();
    ChangeState(TEXT("UpperLayer"), TEXT("Default"));
    ChangeState(TEXT("LowerLayer"), TEXT("Idle"));
    return S_OK;
}

void CPlayerStateMachine::Update(_float DeltaTime)
{
    __super::Update(DeltaTime);
}

_bool CPlayerStateMachine::ChangeState(const _wstring& LayerTag, const _wstring& StateTag)
{
    auto pLayer = FindLayer(LayerTag);
    if (nullptr == pLayer)
        return false;

    _uInt iLayerIndex = GetNumLayer(LayerTag);
    switch (iLayerIndex)
    {
    case 1:
        if (!StateChildChangeAble(pLayer, StateTag))
            return false;
        break;
    }

    if(FAILED(pLayer->ChangeState(StateTag)))
        return false;

    _uInt iStateIndex = pLayer->GetCurrentStateNum();
    //레이어 번호 가져와서 데이터 세팅
    switch(iLayerIndex)
    {
    case 0 :
        m_StateData.eMove_State = MOVE_ACTION(iStateIndex);
        break;
    case 1:
        m_StateData.eMove_Child_State = MOVE_CHILD_ACTION(iStateIndex);
        break;
    }
    return true;
}

_string CPlayerStateMachine::GetStateFullName()
{
    _string FullName = {};
    auto UpperLayer = FindLayer(TEXT("UpperLayer"));
    auto LowerLayer = FindLayer(TEXT("LowerLayer"));

    const char* TopStateName = UpperLayer->GetCurStateName();
    const char* LowStateName = LowerLayer->GetCurStateName();

    if (MOVE_ACTION::DEFAULT != m_StateData.eMove_State)
    {
        if (!m_StateData.bIsAttacking)
            FullName += TopStateName;

        if(MOVE_ACTION::JUMP > m_StateData.eMove_State)
        {
            FullName += "_";
            FullName += LowStateName;
        }
    }
    else
        FullName = LowStateName;

    if (m_StateData.bIsAttacking)
        FullName = "Attack";

    if (MOVE_ACTION::CLIMB == m_StateData.eMove_State || m_StateData.bIsAiming)
    {
        if (MOVE_CHILD_ACTION::IDLE != m_StateData.eMove_Child_State)
        {
            switch (m_StateData.eDireaction)
            {
            case DIREACTION::LEFT:
                FullName += "_Left";
                break;
            case DIREACTION::RIGHT:
                FullName += "_Right";
                break;
            case DIREACTION::BACK:
                FullName += "_Bwd";
                break;
            }

            if (m_StateData.bIsAiming)
                FullName += "_Aim";
        }
    }

    //무기 타입에 의해 네이밍
    switch (m_StateData.eWeaponType)
    {
    case WEAPON::NONE :
    {
        if(!(MOVE_ACTION::CROUCH == m_StateData.eMove_State && MOVE_CHILD_ACTION::WALK == m_StateData.eMove_Child_State))
            FullName += "_None";
    }
        break;
    }
    return FullName;
}

_uInt CPlayerStateMachine::NextStatePhase(const _wstring& LayerTag)
{
    auto pLayer = FindLayer(LayerTag);
    if (nullptr == pLayer)
        return -1;

    return pLayer->StateNextPhase();
}

_uInt CPlayerStateMachine::GetStatePhase(const _wstring& LayerTag)
{
    auto pLayer = FindLayer(LayerTag);
    if (nullptr == pLayer)
        return -1;

    return pLayer->GetCurStatePhase();
}

HRESULT CPlayerStateMachine::ADD_PlayerLayer()
{
    if (FAILED(__super::AddLayer(TEXT("UpperLayer"), CPlayerUpperLayer::Create(ENUM_CLASS(MOVE_ACTION::END)))))
        return E_FAIL;

    if (FAILED(__super::AddLayer(TEXT("LowerLayer"), CPlayerLowerLayer::Create(ENUM_CLASS(MOVE_CHILD_ACTION::END)))))
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

_bool CPlayerStateMachine::StateChildChangeAble(CStateLayer* pLayer, const _wstring& StateTag)
{
    if (MOVE_ACTION::DEFAULT != m_StateData.eMove_State)
    {
        _uInt iStateIndex = pLayer->GetStateNum(StateTag);
        if (ENUM_CLASS(MOVE_CHILD_ACTION::JOG) <= iStateIndex)
            return false;
    }

    return true;
}

CPlayerStateMachine* CPlayerStateMachine::Create()
{
    return new CPlayerStateMachine();
}

void CPlayerStateMachine::Free()
{
    __super::Free();
}
