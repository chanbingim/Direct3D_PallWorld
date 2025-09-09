#include "PlayerCombatLayer.h"
#include "PlayerAttackState.h"

CPlayerCombatLayer::CPlayerCombatLayer()
{
}

CPlayerCombatLayer::CPlayerCombatLayer(const CPlayerCombatLayer& rhs)
{
}

HRESULT CPlayerCombatLayer::Initialize(_uInt iStateSize)
{
    __super::Initialize(iStateSize);
    if (FAILED(ADD_CombatState()))
        return E_FAIL;
    return S_OK;
}

void CPlayerCombatLayer::Update(_float DeltaTime)
{
    __super::Update(DeltaTime);
}

HRESULT CPlayerCombatLayer::ADD_CombatState()
{
    if (FAILED(AddState(TEXT("Attack"), CPlayerAttackState::Create("Attack"))))
        return E_FAIL;

    return S_OK;
}

CPlayerCombatLayer* CPlayerCombatLayer::Create(_uInt iStateSize)
{
    CPlayerCombatLayer* pCombatLayer = new CPlayerCombatLayer();
    if (FAILED(pCombatLayer->Initialize(iStateSize)))
    {
        Safe_Release(pCombatLayer);
        MSG_BOX("CREATE FAIL : PLAYER COMBAT LAYER");
    }
    return pCombatLayer;
}

void CPlayerCombatLayer::Free()
{
    __super::Free();
}
