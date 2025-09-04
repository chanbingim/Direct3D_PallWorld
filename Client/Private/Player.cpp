#include "Player.h"

#include "GameInstance.h"
#include "PlayerStateMachine.h"
#include "PartObject.h"
#include "PlayerCamera.h"

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

    if (FAILED(ADD_PlayerCamera()))
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
    auto UpperBody = FindPartObject(TEXT("Player_Animator"));
    UpperBody->SetAnimIndex(UpperBody->GetAnimIndex(m_pPlayerFSM->GetStateFullName().c_str()));

    __super::Update(fDeletaTime);
}

void CPlayer::Late_Update(_float fDeletaTime)
{
    m_pPlayerCamera->Late_Update(fDeletaTime);
    __super::Late_Update(fDeletaTime);
   
}

HRESULT CPlayer::Render()
{

    return S_OK;
}

void CPlayer::Key_Input(_float fDeletaTime)
{
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();
    if (XMVector3Equal(XMLoadFloat3(&m_PreMovePos), XMVectorZero()))
    {
        m_pPlayerFSM->ChangeState(2, ENUM_CLASS(CPlayerStateMachine::MOVE_CHILD_ACTION::IDLE), TEXT("Idle"));
    }
    else
    {
        if (m_pGameInstance->KeyUp(KEY_INPUT::KEYBOARD, DIK_LSHIFT) || m_pGameInstance->KeyUp(KEY_INPUT::KEYBOARD, MK_CONTROL))
            m_pPlayerFSM->ChangeState(2, ENUM_CLASS(CPlayerStateMachine::MOVE_CHILD_ACTION::JOG), TEXT("Jog"));
    }

    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_LSHIFT))
        m_pPlayerFSM->ChangeState(2, ENUM_CLASS(CPlayerStateMachine::MOVE_CHILD_ACTION::SPRINT), TEXT("Sprint"));
    else if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_LCONTROL))
        m_pPlayerFSM->ChangeState(2, ENUM_CLASS(CPlayerStateMachine::MOVE_CHILD_ACTION::WALK), TEXT("Walk"));

    if (CPlayerStateMachine::NONE_MOVE_ACTION::END == State.eNone_Move_State)
    {
        PlayerMoveView(fDeletaTime);
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
        MovePos = m_pTransformCom->GetLookVector() * 15.f * fDeletaTime;
        m_eDireaction = DIREACTION::FRONT;
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_S))
    {
        MovePos = m_pTransformCom->GetLookVector() * -15.f * fDeletaTime;
        m_eDireaction = DIREACTION::BACK;
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_A))
    {
        MovePos = m_pTransformCom->GetRightVector() * -15.f * fDeletaTime;
        m_eDireaction = DIREACTION::LEFT;
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_D))
    {
        MovePos = m_pTransformCom->GetRightVector() * 15.f * fDeletaTime;
        m_eDireaction = DIREACTION::RIGHT;
        
    }

    if(!XMVector3Equal(MovePos, XMVectorZero()))
        m_pTransformCom->ADD_Position(MovePos);
    XMStoreFloat3(&m_PreMovePos, MovePos);
}

void CPlayer::PlayerMoveView(_float fDeletaTime)
{
    LONG MoveMouseX = m_pGameInstance->GetMouseAxis(0);
    LONG MoveMouseY = m_pGameInstance->GetMouseAxis(1);

    if (-10 > MoveMouseX)
    {
        m_pTransformCom->Turn(m_pTransformCom->GetUpVector(), -XMConvertToRadians(90.f), fDeletaTime);
    }
    else if (10 < MoveMouseX)
    {
        m_pTransformCom->Turn(m_pTransformCom->GetUpVector(), XMConvertToRadians(90.f), fDeletaTime);
    }

    if (-4 > MoveMouseY)
    {
        //m_pTransformCom->Turn(m_pTransformCom->GetRightVector(), 3.f, fDeletaTime);
    }
    else if (4 < MoveMouseY)
    {
        //m_pTransformCom->Turn(m_pTransformCom->GetRightVector(), -3.f, fDeletaTime);
    }

   
}

void CPlayer::ChangeAction(_float fDeltaTime)
{
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();
    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_C))
    {
        if (CPlayerStateMachine::MOVE_ACTION::CROUCH == State.eMove_State)
            m_pPlayerFSM->ChangeState(0, ENUM_CLASS(CPlayerStateMachine::MOVE_ACTION::DEFAULT), TEXT("Default"));
        else
            m_pPlayerFSM->ChangeState(0, ENUM_CLASS(CPlayerStateMachine::MOVE_ACTION::CROUCH), TEXT("Crouch"));
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

HRESULT CPlayer::ADD_PlayerCamera()
{
    CPlayerCamera::PLAYER_CAMERA_DESC Desc;
    ZeroMemory(&Desc, sizeof(CPlayerCamera::PLAYER_CAMERA_DESC));

    Desc.pParent = this;
    lstrcpy(Desc.ObjectTag, L"PlayerCamera");

    Desc.fFar = 1000.f;
    Desc.fNear = 0.1f;
    Desc.fFov = XMConvertToRadians(90.f);

    Desc.vEye = { 30.f, 10.f, -60.f };
    Desc.vAt = { 30.f, 10.f, 1.f };

    auto UpperBody = FindPartObject(TEXT("Player_Animator"));
    Desc.pSocketMatrix = UpperBody->GetBoneMatrix("neck_01");

    m_pPlayerCamera = CPlayerCamera::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(m_pPlayerCamera->Initialize(&Desc)))
        return E_FAIL;

    __super::ADD_Child(m_pPlayerCamera);
    return S_OK;
}

HRESULT CPlayer::ADD_PartObejcts()
{
    CPartObject::PARTOBJECT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CPartObject::PARTOBJECT_DESC));

    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };

    lstrcpy(Desc.ObjectTag, L"Player_Animator");
    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_PartData_Default"), TEXT("Player_Animator"), &Desc)))
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
    Safe_Release(m_pPlayerCamera);
}
