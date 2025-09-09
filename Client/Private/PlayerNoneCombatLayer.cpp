#include "PlayerNoneCombatLayer.h"

CPlayerNoneCombatLayer::CPlayerNoneCombatLayer() 
{
}

CPlayerNoneCombatLayer::CPlayerNoneCombatLayer(const CPlayerNoneCombatLayer& rhs)
{
   
}

HRESULT CPlayerNoneCombatLayer::Initialize(_uInt iStateSize)
{
    __super::Initialize(iStateSize);
    if (FAILED(ADD_CombatState()))
        return E_FAIL;
    return S_OK;
}

void CPlayerNoneCombatLayer::Update(_float DeltaTime)
{
    __super::Update(DeltaTime);
}

HRESULT CPlayerNoneCombatLayer::ADD_CombatState()
{
    return S_OK;
}

CPlayerNoneCombatLayer* CPlayerNoneCombatLayer::Create(_uInt iStateSize)
{
    CPlayerNoneCombatLayer* pCombatLayer = new CPlayerNoneCombatLayer();
    if (FAILED(pCombatLayer->Initialize(iStateSize)))
    {
        Safe_Release(pCombatLayer);
        MSG_BOX("CREATE FAIL : PLAYER COMBAT LAYER");
    }
    return pCombatLayer;
}

void CPlayerNoneCombatLayer::Free()
{
    __super::Free();
}