#include "PlayerNoneCombatLayer.h"

CPlayerNoneCombatLayer::CPlayerNoneCombatLayer() 
{
}

CPlayerNoneCombatLayer::CPlayerNoneCombatLayer(const CPlayerNoneCombatLayer& rhs)
{
   
}

HRESULT CPlayerNoneCombatLayer::Initialize(void* pArg, _uInt iStateSize)
{
    __super::Initialize(pArg, iStateSize);
    if (FAILED(ADD_CombatState()))
        return E_FAIL;
    return S_OK;
}

void CPlayerNoneCombatLayer::Update(_float DeltaTime, void* pArg)
{
    __super::Update(DeltaTime, pArg);
}

HRESULT CPlayerNoneCombatLayer::ADD_CombatState()
{
    return S_OK;
}

CPlayerNoneCombatLayer* CPlayerNoneCombatLayer::Create(void* pArg, _uInt iStateSize)
{
    CPlayerNoneCombatLayer* pCombatLayer = new CPlayerNoneCombatLayer();
    if (FAILED(pCombatLayer->Initialize(pArg, iStateSize)))
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