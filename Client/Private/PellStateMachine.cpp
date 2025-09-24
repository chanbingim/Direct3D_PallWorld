#include "PellStateMachine.h"

#include "ItemStruct.h"

#pragma region LAYER
#include "PellCombatLayer.h"
#include "PellMoveLayer.h"
#include "ContainerObject.h"
#pragma endregion

CPellStateMachine::CPellStateMachine()
{
}

CPellStateMachine::CPellStateMachine(const CPellStateMachine& rhs)
{
}

HRESULT CPellStateMachine::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    PELLFSM_DESC* pDesc = static_cast<PELLFSM_DESC*>(pArg);
    m_pOwner = pDesc->pOwner;

    if (FAILED(ADD_StateLayer()))
        return E_FAIL;

    if (FAILED(SetUp_PellInitState()))
        return E_FAIL;

    ChangeState(TEXT("BodyLayer"), TEXT("Idle"));

    return S_OK;
}

void CPellStateMachine::Update(_float DeltaTime, void* pArg)
{
    __super::Update(DeltaTime, pArg);
}

_bool CPellStateMachine::ChangeState(const _wstring& LayerTag, const _wstring& StateTag, void* pArg)
{
    auto pLayer = FindLayer(LayerTag);
    if (nullptr == pLayer)
        return false;

    _uInt iLayerIndex = GetNumLayer(LayerTag);

    if (FAILED(pLayer->ChangeState(StateTag, pArg)))
        return false;

    _uInt iStateIndex = pLayer->GetCurrentStateNum();
    //레이어 번호 가져와서 데이터 세팅
    switch (iLayerIndex)
    {
    case 0:
        m_StateData.eMove_State = MOVE_ACTION(iStateIndex);
        break;
    case 1:
        m_StateData.eCombat_State = COMBAT_ACTION(iStateIndex);
        break;
    }

    return true;

}

void CPellStateMachine::PellStateReset(const _wstring& LayerTag)
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
        m_StateData.eCombat_State = COMBAT_ACTION::END;
        break;
    }

    ResetLayer(LayerTag);
}

_bool CPellStateMachine::GetLayerLastPhase(const _wstring& LayerTag)
{
    auto pLayer = FindLayer(LayerTag);
    if (nullptr == pLayer)
        return false;

    return pLayer->GetCurrentStateLastPhase();
}

_string CPellStateMachine::GetAnimationName()
{
    _string FullName = {};
    auto BodyLayer = FindLayer(TEXT("BodyLayer"));
    auto CombatLayer = FindLayer(TEXT("CombatLayer"));

    if (nullptr == BodyLayer || nullptr == CombatLayer)
        return "";

    const char* BodyAnimName = BodyLayer->GetCurStateName();
    const char* CombatAnimName = CombatLayer->GetCurStateName();

    if (COMBAT_ACTION::END == m_StateData.eCombat_State )
    {
        if(nullptr == BodyAnimName)
            return "";

        FullName = BodyAnimName;
    }
    else
    {
        if (nullptr == CombatAnimName)
            return "";

        FullName = CombatAnimName;
    }
   
    return FullName;
}

void CPellStateMachine::CombatStateReset()
{
    m_StateData.eCombat_State = COMBAT_ACTION::END;
    m_StateData.bIsCombat = false;
    ResetLayer(TEXT("CombatLayer"));
}

HRESULT CPellStateMachine::ADD_StateLayer()
{
    CPellMoveLayer::PELL_LAYER_DESC MoveDesc;
    MoveDesc.pOwner = m_pOwner;
    if (FAILED(__super::AddLayer(TEXT("BodyLayer"), CPellMoveLayer::Create(&MoveDesc, ENUM_CLASS(MOVE_ACTION::END)))))
        return E_FAIL;

    if (FAILED(__super::AddLayer(TEXT("CombatLayer"), CPellCombatLayer::Create(nullptr, ENUM_CLASS(COMBAT_ACTION::END)))))
        return E_FAIL;

    return S_OK;
}

HRESULT CPellStateMachine::SetUp_PellInitState(void* pArg)
{
    if (nullptr == pArg)
    {
        //플레이어가 데이터를 외부에서 받아오지 않았을경우
        //초기 데이터 세팅을 위해 데이터를 세팅해준다.
        m_StateData.bIsAiming = false;
        m_StateData.bIsAttacking = false;

        m_StateData.iWeaponType = ENUM_CLASS(WEAPON::NONE);
        m_StateData.eMove_State = MOVE_ACTION::DEFAULT;
        m_StateData.eCombat_State = COMBAT_ACTION::END;
    }

    PELL_STATE* Desc = static_cast<PELL_STATE*>(pArg);
    if (Desc)
    {

    }

    return S_OK;
}

CPellStateMachine* CPellStateMachine::Create()
{
    return new CPellStateMachine();
}

void CPellStateMachine::Free()
{
    __super::Free();

}
