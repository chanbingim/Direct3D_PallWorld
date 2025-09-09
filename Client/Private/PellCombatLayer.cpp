#include "PellCombatLayer.h"

#pragma region State
#include "PellAttackState.h"
#include "PellHitState.h"
#pragma endregion

CPellCombatLayer::CPellCombatLayer()
{
}

CPellCombatLayer::CPellCombatLayer(const CPellCombatLayer& rhs)
{
}

HRESULT CPellCombatLayer::Initialize(_uInt iStateSize)
{
    __super::Initialize(iStateSize);

    if (FAILED(ADD_CombatState()))
        return E_FAIL;

    return S_OK;
}

void CPellCombatLayer::Update(_float DeltaTime)
{
    __super::Update(DeltaTime);
}

HRESULT CPellCombatLayer::ADD_CombatState()
{
    if (FAILED(AddState(TEXT("Attack"), CPellAttackState::Create("Attack"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Hit"), CPellAttackState::Create("Hit"))))
        return E_FAIL;

    return S_OK;
}

CPellCombatLayer* CPellCombatLayer::Create(_uInt iStateSize)
{
    return new CPellCombatLayer();
}

void CPellCombatLayer::Free()
{
    __super::Free();
}
