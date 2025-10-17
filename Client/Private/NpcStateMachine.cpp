#include "NpcStateMachine.h"

#include "GameInstance.h"
#include "DialogLayer.h"

CNpcStateMachine::CNpcStateMachine() :
    CFiniteStateMachine()
{
}

CNpcStateMachine::CNpcStateMachine(const CNpcStateMachine& rhs) :
    CFiniteStateMachine(rhs)
{
}

HRESULT CNpcStateMachine::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    if (FAILED(ADD_NpcLayer()))
        return E_FAIL;

    SettingNpcState();
    ChangeState(TEXT("Dialog_Layer"), TEXT("Idle"));


    return S_OK;
}

void CNpcStateMachine::Update(_float DeltaTime, void* pArg)
{
    __super::Update(DeltaTime, pArg);
}

_bool CNpcStateMachine::ChangeState(const _wstring& LayerTag, const _wstring& StateTag, void* pArg)
{
    auto pLayer = FindLayer(LayerTag);
    if (nullptr == pLayer)
        return false;

    _uInt iLayerIndex = GetNumLayer(LayerTag);
    if (FAILED(pLayer->ChangeState(StateTag, pArg)))
        return false;

    _uInt iStateIndex = pLayer->GetCurrentStateNum();
    //레이어 번호 가져와서 데이터 세팅
    switch (iLayerIndex)
    {
    case 0:
        m_StateData.eNpc_DialogState = NPC_DIALOG_ACTION(iStateIndex);
        break;
    }
    return true;
}

_string CNpcStateMachine::GetStateFullName()
{
    _string StateName = {};
    auto pLayer = FindLayer(TEXT("Dialog_Layer"));
    if (nullptr == pLayer)
        return "";

    if (pLayer->GetCurStateName())
        StateName = pLayer->GetCurStateName();
    
    return StateName;
}

_bool CNpcStateMachine::GetLayerLastPhase(const _wstring& LayerTag)
{
    auto pLayer = FindLayer(LayerTag.c_str());
    if (nullptr == pLayer)
        return false;

    return pLayer->GetCurrentStateLastPhase();
}

void CNpcStateMachine::NpcStateReset(const _wstring& LayerTag)
{
    auto pLayer = FindLayer(LayerTag.c_str());
    if (nullptr == pLayer)
        return;

    pLayer->ResetLayer();
}

HRESULT CNpcStateMachine::ADD_NpcLayer()
{
    if (FAILED(__super::AddLayer(TEXT("Dialog_Layer"), CDialogLayer::Create(nullptr, ENUM_CLASS(NPC_DIALOG_ACTION::END)))))
        return E_FAIL;

    return S_OK;
}

void CNpcStateMachine::SettingNpcState(void* pArg)
{
    if (nullptr == pArg)
    {
        //플레이어가 데이터를 외부에서 받아오지 않았을경우
        //초기 데이터 세팅을 위해 데이터를 세팅해준다.
        m_StateData.eNpc_DialogState = NPC_DIALOG_ACTION::IDLE;
        return;
    }
}

CNpcStateMachine* CNpcStateMachine::Create()
{
    return new CNpcStateMachine();
}

void CNpcStateMachine::Free()
{
    __super::Free();
}
