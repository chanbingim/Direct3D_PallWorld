#include "PlayerUpperLayer.h"

#pragma region Parent State
#include "CrouchState.h"
#include "ClimbState.h"
#include "JumpState.h"
#include "PlayerIdleState.h"

#pragma endregion

CPlayerUpperLayer::CPlayerUpperLayer()
{
}

CPlayerUpperLayer::CPlayerUpperLayer(const CPlayerUpperLayer& rhs)
{
}

HRESULT CPlayerUpperLayer::Initialize(_uInt iStateSize)
{
    __super::Initialize(iStateSize);
    if (FAILED(ADD_UpperState()))
        return E_FAIL;
    return S_OK;
}

void CPlayerUpperLayer::Update(_float DeltaTime)
{
    __super::Update(DeltaTime);
}

HRESULT CPlayerUpperLayer::ADD_UpperState()
{
    if (FAILED(AddState(TEXT("Default"), CPlayerIdleState::Create(""))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Crouch"), CCrouchState::Create("Crouch"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Climb"), CClimbState::Create("Climb"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Jump"), CJumpState::Create("Jump"))))
        return E_FAIL;

    return S_OK;
}

CPlayerUpperLayer* CPlayerUpperLayer::Create(_uInt iStateSize)
{
    CPlayerUpperLayer* pUpperLayer = new CPlayerUpperLayer();
    if (FAILED(pUpperLayer->Initialize(iStateSize)))
    {
        Safe_Release(pUpperLayer);
        MSG_BOX("CREATE FAIL : PLAYER UPPER LAYER");
    }
    return pUpperLayer;
}

void CPlayerUpperLayer::Free()
{
    __super::Free();
}
