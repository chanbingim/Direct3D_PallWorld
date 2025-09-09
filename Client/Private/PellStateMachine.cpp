#include "PellStateMachine.h"

#include "ItemStruct.h"

#pragma region LAYER
#include "PellCombatLayer.h"
#include "PellMoveLayer.h"
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

    if (FAILED(ADD_StateLayer()))
        return E_FAIL;

    ChangeState(TEXT("MoveLayer"), TEXT("IDLE"));

    return S_OK;
}

void CPellStateMachine::Update(_float DeltaTime)
{
    __super::Update(DeltaTime);
}

_bool CPellStateMachine::ChangeState(const _wstring& LayerTag, const _wstring& StateTag)
{
    return _bool();
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

HRESULT CPellStateMachine::ADD_StateLayer()
{
    if (FAILED(__super::AddLayer(TEXT("MoveLayer"), CPellMoveLayer::Create(ENUM_CLASS(MOVE_ACTION::END)))))
        return E_FAIL;

    if (FAILED(__super::AddLayer(TEXT("CombatLayer"), CPellCombatLayer::Create(ENUM_CLASS(COMBAT_ACTION::END)))))
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
