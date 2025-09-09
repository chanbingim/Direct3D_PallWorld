#include "PellMoveLayer.h"

#pragma region State
#include "PellIdleState.h"
#include "PellPatrolState.h"
#include "PellResetState.h"
#include "PellWorkState.h"
#pragma endregion


CPellMoveLayer::CPellMoveLayer()
{
}

CPellMoveLayer::CPellMoveLayer(const CPellMoveLayer& rhs)
{
}

HRESULT CPellMoveLayer::Initialize(_uInt iStateSize)
{
    __super::Initialize(iStateSize);
    if (FAILED(ADD_MoveState()))
        return E_FAIL;

    return E_NOTIMPL;
}

void CPellMoveLayer::Update(_float DeltaTime)
{
    __super::Update(DeltaTime);
}

HRESULT CPellMoveLayer::ADD_MoveState()
{
    if (FAILED(AddState(TEXT("Idle"), CPellIdleState::Create("Idle"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Patrol"), CPellPatrolState::Create("Patrol"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Reset"), CPellResetState::Create("Reset"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Work"), CPellWorkState::Create("Work"))))
        return E_FAIL;

    return S_OK;
}

CPellMoveLayer* CPellMoveLayer::Create(_uInt iStateSize)
{
    return new CPellMoveLayer();
}

void CPellMoveLayer::Free()
{
    __super::Free();
}
