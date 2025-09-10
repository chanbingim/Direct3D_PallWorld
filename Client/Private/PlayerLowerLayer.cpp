#include "PlayerLowerLayer.h"

#pragma region Child State
#include "PlayerWalkState.h"
#include "PlayerChildIdleState.h"
#include "PlayerJog.h"
#include "PlayerSprint.h"
#pragma endregion

CPlayerLowerLayer::CPlayerLowerLayer()
{
}

CPlayerLowerLayer::CPlayerLowerLayer(const CPlayerLowerLayer& rhs)
{
}

HRESULT CPlayerLowerLayer::Initialize(void* pArg, _uInt iStateSize)
{
    __super::Initialize(pArg, iStateSize);
    if (FAILED(ADD_LowerState()))
        return E_FAIL;

    return S_OK;
}

void CPlayerLowerLayer::Update(_float DeltaTime)
{
    __super::Update(DeltaTime);
}

HRESULT CPlayerLowerLayer::ADD_LowerState()
{
    if (FAILED(AddState(TEXT("Walk"), CPlayerWalkState::Create("Walk"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Idle"), CPlayerChildIdleState::Create("Idle"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Jog"), CPlayerJogState::Create("Jog"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Sprint"), CPlayerSprintState::Create("Sprint"))))
        return E_FAIL;
    return S_OK;
}

CPlayerLowerLayer* CPlayerLowerLayer::Create(void* pArg, _uInt iStateSize)
{
    CPlayerLowerLayer* pLowerLayer =  new CPlayerLowerLayer();
    if (FAILED(pLowerLayer->Initialize(pArg, iStateSize)))
    {
        Safe_Release(pLowerLayer);
        MSG_BOX("CREATE FAIL : PLAYER LOWER LAYER");
    }
    return pLowerLayer;
}

void CPlayerLowerLayer::Free()
{
    __super::Free();
}
