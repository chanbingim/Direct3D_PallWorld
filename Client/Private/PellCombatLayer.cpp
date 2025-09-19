#include "PellCombatLayer.h"

#pragma region State
#include "PellAttackState.h"
#include "PellHitState.h"
#include "PellDeadState.h"
#pragma endregion

CPellCombatLayer::CPellCombatLayer()
{
}

CPellCombatLayer::CPellCombatLayer(const CPellCombatLayer& rhs)
{
}

HRESULT CPellCombatLayer::Initialize(void* pArg, _uInt iStateSize)
{
    __super::Initialize(pArg, iStateSize);

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

    if (FAILED(AddState(TEXT("Hit"), CPellHitState::Create("Damage"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Dead"), CPellDeadState::Create("Dead"))))
        return E_FAIL;

    return S_OK;
}

CPellCombatLayer* CPellCombatLayer::Create(void* pArg, _uInt iStateSize)
{
    CPellCombatLayer* pPellCombatLayer = new CPellCombatLayer();
    if (FAILED(pPellCombatLayer->Initialize(pArg, iStateSize)))
    {
        Safe_Release(pPellCombatLayer);
        MSG_BOX("CREATE FAIL : PELL COMBAT LAYER");
    }

    return pPellCombatLayer;
}

void CPellCombatLayer::Free()
{
    __super::Free();
}
