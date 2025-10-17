#include "DialogLayer.h"

#pragma region Child State
#include "NpcIdleState.h"
#include "NpcListenState.h"
#include "NpcTalkState.h"
#pragma endregion

CDialogLayer::CDialogLayer()
{
}

CDialogLayer::CDialogLayer(const CDialogLayer& rhs)
{
   
}

HRESULT CDialogLayer::Initialize(void* pArg, _uInt iStateSize)
{
    __super::Initialize(pArg, iStateSize);
    if (FAILED(ADD_State()))
        return E_FAIL;

    return S_OK;
}

void CDialogLayer::Update(_float DeltaTime, void* pArg)
{
    __super::Update(DeltaTime, pArg);
}

HRESULT CDialogLayer::ADD_State()
{
    if (FAILED(AddState(TEXT("Idle"), CNpcIdleState::Create("Idle"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Talking"), CNpcTalkState::Create("Talking"))))
        return E_FAIL;

    if (FAILED(AddState(TEXT("Listen"), CNpcListenState::Create("Listen"))))
        return E_FAIL;

    return S_OK;
}

CDialogLayer* CDialogLayer::Create(void* pArg, _uInt iStateSize)
{
    CDialogLayer* pDialogLayer = new CDialogLayer();
    if (FAILED(pDialogLayer->Initialize(pArg, iStateSize)))
    {
        Safe_Release(pDialogLayer);
        MSG_BOX("CREATE FAIL : NPC Dialog Layer");
    }
    return pDialogLayer;
}

void CDialogLayer::Free()
{
    __super::Free();
}
