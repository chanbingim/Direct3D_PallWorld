#include "PellMoveLayer.h"

#include "PellBase.h"

#pragma region State
#include "PellIdleState.h"
#include "PellPatrolState.h"
#include "PellResetState.h"
#include "PellWorkState.h"
#include "PellCarryState.h"
#include "PellStateLaunched.h"
#pragma endregion


CPellMoveLayer::CPellMoveLayer()
{
}

CPellMoveLayer::CPellMoveLayer(const CPellMoveLayer& rhs)
{
}

HRESULT CPellMoveLayer::Initialize(void* pArg, _uInt iStateSize)
{
    __super::Initialize(pArg, iStateSize);
    PELL_LAYER_DESC* pPellLayerDesc = static_cast<PELL_LAYER_DESC*>(pArg);
    m_pOwner = pPellLayerDesc->pOwner;

    if (FAILED(ADD_MoveState()))
        return E_FAIL;

    return S_OK;
}

void CPellMoveLayer::Update(_float DeltaTime, void* pArg)
{
    if (m_pCurState)
        m_pCurState->OnStateExcution(DeltaTime, pArg);
}

HRESULT CPellMoveLayer::ADD_MoveState()
{
    if (FAILED(AddState(TEXT("Idle"), CPellIdleState::Create("Idle"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Patrol"), CPellPatrolState::Create("Patrol"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Rest"), CPellResetState::Create("Rest"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Work"), CPellWorkState::Create("Work", static_cast<CPellBase *>(m_pOwner)))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Carry"), CPellCarryState::Create("Carry"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Launched"), CPellStateLaunched::Create("Launched"))))
        return E_FAIL;

    return S_OK;
}

CPellMoveLayer* CPellMoveLayer::Create(void* pArg, _uInt iStateSize)
{
    CPellMoveLayer* pMoveLayer = new CPellMoveLayer();
    if (FAILED(pMoveLayer->Initialize(pArg, iStateSize)))
    {
        Safe_Release(pMoveLayer);
        MSG_BOX("CREATE FAIL : PELL MOVE LAYER");
    }

    return pMoveLayer;
}

void CPellMoveLayer::Free()
{
    __super::Free();
}
