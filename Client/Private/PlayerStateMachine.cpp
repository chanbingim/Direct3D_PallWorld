#include "PlayerStateMachine.h"

#include "StringHelper.h"

#include "PlayerLowerLayer.h"
#include "PlayerUpperLayer.h"
#include "PlayerCombatLayer.h"
#include "PlayerNoneCombatLayer.h"
#include "ItemStruct.h"

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

void CPlayerStateMachine::Update(_float DeltaTime, void* pArg)
{
    __super::Update(DeltaTime, pArg);
}

_bool CPlayerStateMachine::ChangeState(const _wstring& LayerTag, const _wstring& StateTag, void* pArg)
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

    if(FAILED(pLayer->ChangeState(StateTag, pArg)))
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
    case 2:
        m_StateData.eCombat_State = COMBAT_ACTION(iStateIndex);
        break;
    case 3:
        m_StateData.eNone_Combat_State = NONE_COBAT_ACTION(iStateIndex);
        break;
    }
    return true;
}

_string CPlayerStateMachine::GetStateFullName()
{
    _string FullName = {};
    auto UpperLayer = FindLayer(TEXT("UpperLayer"));
    auto LowerLayer = FindLayer(TEXT("LowerLayer"));
    auto NoneCombatLayer = FindLayer(TEXT("None_Combat_Layer"));

    const char* TopStateName = UpperLayer->GetCurStateName();
    const char* LowStateName = LowerLayer->GetCurStateName();
    const char* NoneCombatStateName = NoneCombatLayer->GetCurStateName();

    if (NONE_COBAT_ACTION::END == m_StateData.eNone_Combat_State)
    {
        if (MOVE_ACTION::DEFAULT != m_StateData.eMove_State)
        {
            FullName += TopStateName;

            if (MOVE_ACTION::JUMP > m_StateData.eMove_State)
            {
                FullName += "_";
                FullName += LowStateName;
            }
        }
        else
            FullName = LowStateName;

        if (MOVE_ACTION::DEFAULT == m_StateData.eMove_State && MOVE_CHILD_ACTION::IDLE == m_StateData.eMove_Child_State)
        {
            if (m_StateData.bIsAttacking)
            {
                if (m_StateData.bIsPallCarry)
                    FullName = "Throw";
                else
                    FullName = "Attack";
            }
                

            if (COMBAT_ACTION::ATTACK == m_StateData.eCombat_State)
            {
                auto CombatLayer = FindLayer(TEXT("CombatLayer"));
                const char* CombatStateName = CombatLayer->GetCurStateName();
                if (CombatStateName)
                    FullName = CombatStateName;
            }
        }

        if (!m_StateData.bIsPallCarry && m_StateData.bIsAiming)
        {
            if (MOVE_CHILD_ACTION::IDLE != m_StateData.eMove_Child_State)
            {
                switch (m_StateData.eDireaction)
                {
                case DIRECTION::FRONT:
                {
                    if (MOVE_ACTION::CLIMB == m_StateData.eMove_State)
                        FullName += "_Up";
                    else
                        FullName += "_Fwd";
                }
                break;
                case DIRECTION::LEFT:
                    FullName += "_Left";
                    break;
                case DIRECTION::RIGHT:
                    FullName += "_Right";
                    break;
                case DIRECTION::BACK:
                {
                    if (MOVE_ACTION::CLIMB == m_StateData.eMove_State)
                        FullName += "_Down";
                    else
                        FullName += "_Bwd";
                }
                break;
                }

                if (m_StateData.bIsAiming)
                    FullName += "_Aim";
            }
        }

        if (!m_StateData.bIsPallCarry)
        {
            const char* WeaponName = GetWeaponName(m_StateData.iWeaponType);
            if (WeaponName)
                FullName += WeaponName;
        }
        else
            FullName += "_PalCarry";
    }
    else
        FullName = NoneCombatStateName;
   
    return FullName;
}

_string CPlayerStateMachine::GetLayerAimStateName()
{
    _string FullName = {};
    auto UpperLayer = FindLayer(TEXT("UpperLayer"));
    auto LowerLayer = FindLayer(TEXT("LowerLayer"));
    auto CombatLayer = FindLayer(TEXT("CombatLayer"));
 

    const char* TopStateName = UpperLayer->GetCurStateName();
    const char* LowStateName = "Idle";
    const char* CombatStateName = CombatLayer->GetCurStateName();

    if (MOVE_ACTION::DEFAULT != m_StateData.eMove_State)
    {
        if (!m_StateData.bIsAttacking)
            FullName += TopStateName;

        if (MOVE_ACTION::JUMP > m_StateData.eMove_State)
        {
            FullName += "_";
            FullName += LowStateName;
        }
    }
    else
        FullName = LowStateName;

    if (m_StateData.bIsAttacking)
        FullName = "Attack";

    const char* WeaponName = GetWeaponName(m_StateData.iWeaponType);
    if (WeaponName)
        FullName += WeaponName;

    return FullName;
}

_bool CPlayerStateMachine::GetLayerLastPhase(const _wstring& LayerTag)
{
    auto pLayer = FindLayer(LayerTag);
    if (nullptr == pLayer)
        return false;

    return pLayer->GetCurrentStateLastPhase();
}

void CPlayerStateMachine::PlayerStateReset(const _wstring& LayerTag)
{
    auto pLayer = FindLayer(LayerTag);
    if (nullptr == pLayer)
        return;

    _uInt iLayerIndex = GetNumLayer(LayerTag);
    _uInt iStateIndex = pLayer->GetCurrentStateNum();
    //레이어 번호 가져와서 데이터 세팅
    switch (iLayerIndex)
    {
    case 0:
        m_StateData.eMove_State = MOVE_ACTION::DEFAULT;
        break;
    case 1:
        m_StateData.eMove_Child_State = MOVE_CHILD_ACTION::IDLE;
        break;
    case 2:
        m_StateData.eCombat_State = COMBAT_ACTION::END;
        break;
    case 3:
        m_StateData.eNone_Combat_State = NONE_COBAT_ACTION::END;
        break;
    }
    ResetLayer(LayerTag);
}

HRESULT CPlayerStateMachine::ADD_PlayerLayer()
{
    if (FAILED(__super::AddLayer(TEXT("UpperLayer"), CPlayerUpperLayer::Create(nullptr, ENUM_CLASS(MOVE_ACTION::END)))))
        return E_FAIL;

    if (FAILED(__super::AddLayer(TEXT("LowerLayer"), CPlayerLowerLayer::Create(nullptr, ENUM_CLASS(MOVE_CHILD_ACTION::END)))))
        return E_FAIL;

    if (FAILED(__super::AddLayer(TEXT("CombatLayer"), CPlayerCombatLayer::Create(nullptr, ENUM_CLASS(COMBAT_ACTION::END)))))
        return E_FAIL;

    if (FAILED(__super::AddLayer(TEXT("None_Combat_Layer"), CPlayerNoneCombatLayer::Create(nullptr, ENUM_CLASS(NONE_COBAT_ACTION::END)))))
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

        m_StateData.iWeaponType = ENUM_CLASS(WEAPON::NONE);
        m_StateData.eMove_State = MOVE_ACTION::DEFAULT;
        m_StateData.eMove_Child_State = MOVE_CHILD_ACTION::IDLE;
        m_StateData.eCombat_State = COMBAT_ACTION::END;
        m_StateData.eNone_Combat_State = NONE_COBAT_ACTION::END;
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

const char* CPlayerStateMachine::GetWeaponName(_uInt eWeaponType)
{
    //무기 타입에 의해 네이밍
    switch (WEAPON(eWeaponType))
    {
    case WEAPON::NONE:
    {
        if (!(MOVE_ACTION::CROUCH == m_StateData.eMove_State && MOVE_CHILD_ACTION::WALK == m_StateData.eMove_Child_State))
            return "_None";
    }
    case WEAPON::MELEE:
        return "_Melee";

    case WEAPON::BOW :
        return "_Bow";

    case WEAPON::THROW:
        return "_Throw";
    }

    return nullptr;
}

CPlayerStateMachine* CPlayerStateMachine::Create()
{
    return new CPlayerStateMachine();
}

void CPlayerStateMachine::Free()
{
    __super::Free();
}
