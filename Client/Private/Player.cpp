#include "Player.h"

#include "GameInstance.h"
#include "PlayerStateMachine.h"
#include "PartObject.h"

CPlayer::CPlayer(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CContainerObject(pGraphic_Device, pDeviceContext)
{
}

CPlayer::CPlayer(const CPlayer& rhs) :
    CContainerObject(rhs)
{
}

HRESULT CPlayer::Initalize_Prototype()
{
   

    return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_PartObejcts()))
        return E_FAIL;

    if (FAILED(ADD_PlayerStateMachine()))
        return E_FAIL;

    if(m_ObejctTag.c_str() == L"")
        m_ObejctTag = TEXT("Main Player");

    return S_OK;
}

void CPlayer::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
    if (GAMEMODE::GAME == m_pGameInstance->GetGameMode())
        Key_Input(fDeletaTime);
        
}

void CPlayer::Update(_float fDeletaTime)
{
    m_pPlayerFSM->Update(fDeletaTime);
    auto UpperBody = FindPartObject(TEXT("Part_UpperBody"));
    UpperBody->SetAnimIndex(UpperBody->GetAnimIndex(m_pPlayerFSM->GetStateFullName().c_str()));

    __super::Update(fDeletaTime);
}

void CPlayer::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);

}

HRESULT CPlayer::Render()
{

    return S_OK;
}

void CPlayer::Key_Input(_float fDeletaTime)
{
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();
    if (m_pGameInstance->KeyUp(KEY_INPUT::KEYBOARD, DIK_LSHIFT) || m_pGameInstance->KeyUp(KEY_INPUT::KEYBOARD, MK_CONTROL))
        m_pPlayerFSM->ChangeState(2, TEXT("Jog"));

    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_LSHIFT))
        m_pPlayerFSM->ChangeState(2, TEXT("Sprint"));
    else if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_LCONTROL))
        m_pPlayerFSM->ChangeState(2, TEXT("Walk"));

    if (CPlayerStateMachine::NONE_MOVE_ACTION::END == State.eNone_Move_State)
    {
        ChangeAction(fDeletaTime);
        MoveAction(fDeletaTime);
    }
    else
    {

    }
}

void CPlayer::MoveAction(_float fDeletaTime)
{
    _vector MovePos = {};
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_W))
    {
        _vector MovePos = m_pTransformCom->GetLookVector() * 5.f * fDeletaTime;
        m_eDireaction = DIREACTION::FRONT;
        m_pTransformCom->ADD_Position(MovePos);
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_S))
    {
        MovePos = m_pTransformCom->GetLookVector() * -5.f * fDeletaTime;
        m_eDireaction = DIREACTION::BACK;
        m_pTransformCom->ADD_Position(MovePos);
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_A))
    {
        MovePos = m_pTransformCom->GetRightVector() * -5.f * fDeletaTime;
        m_eDireaction = DIREACTION::LEFT;
        //m_pTransformCom->Turn(m_pTransformCom->GetUpVector(), 5.f, fDeletaTime);
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_D))
    {
        MovePos = m_pTransformCom->GetRightVector() * 5.f * fDeletaTime;
        m_eDireaction = DIREACTION::RIGHT;
    }
}

void CPlayer::ChangeAction(_float fDeltaTime)
{
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();
    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_C))
    {
        if (CPlayerStateMachine::MOVE_ACTION::CROUCH == State.eMove_State)
            m_pPlayerFSM->ChangeState(0, L"Default");
        else
            m_pPlayerFSM->ChangeState(0, L"Crouch");
    }

    if (WEAPON::NONE != State.eWeaponType)
    {
        if (m_pGameInstance->KeyPressed(KEY_INPUT::MOUSE, 1))
            m_pPlayerFSM->SetAiming(true);
        else if(m_pGameInstance->KeyUp(KEY_INPUT::MOUSE, 1))
            m_pPlayerFSM->SetAiming(false);
    }
}

HRESULT CPlayer::ADD_PlayerStateMachine()
{
    //여기서 플레이어가 데이터를 읽어서 이전에 데이터를 저장했다면
    //데이터를 세팅해준다.
    m_pPlayerFSM = CPlayerStateMachine::Create();
    if (nullptr == m_pPlayerFSM)
        return E_FAIL;

    if (FAILED(m_pPlayerFSM->Initialize()))
    {
        Safe_Release(m_pPlayerFSM);
        MSG_BOX("CREATE FAIL : STATE MACHNIE");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CPlayer::ADD_PartObejcts()
{
    CGameObject::GAMEOBJECT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CGameObject::GAMEOBJECT_DESC));

    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };

    lstrcpy(Desc.ObjectTag, L"Player_body");
    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Upper_Body_Default"), TEXT("Part_UpperBody"), &Desc)))
        return E_FAIL;
    
    return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CPlayer* pPlayer = new CPlayer(pGraphic_Device, pDeviceContext);
    if (FAILED(pPlayer->Initalize_Prototype()))
    {
        Safe_Release(pPlayer);
        MSG_BOX("CREATE FAIL : PLAYER");
    }
    return pPlayer;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CPlayer* pPlayer = new CPlayer(*this);
    if (FAILED(pPlayer->Initialize(pArg)))
    {
        Safe_Release(pPlayer);
        MSG_BOX("CLONE FAIL : PLAYER");
    }
    return pPlayer;
}

void CPlayer::Free()
{
    __super::Free();

    Safe_Release(m_pPlayerFSM);
}
