#include "Player.h"

#include "GameInstance.h"

#include "PlayerStateMachine.h"
#include "State.h"

#include "PlayerPartData.h"
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

    if(m_ViewCamera)
        m_pPlayerCamera->Late_Update(fDeletaTime);

    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_F10))
        m_ViewCamera = !m_ViewCamera;



  if (GAMEMODE::GAME == m_pGameInstance->GetGameMode())
  {
      
      Key_Input(fDeletaTime);
  }
       
        
}

void CPlayer::Update(_float fDeletaTime)
{
    m_pPlayerFSM->Update(fDeletaTime);
    auto UpperBody = FindPartObject(TEXT("Player_Animator"));
    UpperBody->SetAnimIndex(UpperBody->GetAnimIndex(m_pPlayerFSM->GetStateFullName().c_str()), m_bIsAnimLoop);

    UpdatePlayerAction(fDeletaTime);
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
    if (XMVector3Equal(XMLoadFloat3(&m_PreMovePos), XMVectorZero()))
    {
        m_pPlayerFSM->ChangeState(TEXT("LowerLayer"), TEXT("Idle"));
    }
    else
    {
        if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_LSHIFT))
        {
            if(m_pPlayerFSM->ChangeState(TEXT("LowerLayer"), TEXT("Sprint")))
                m_fMoveSpeed = P_RUN_SPEED;
        }
        else if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_LCONTROL))
        {
            if(m_pPlayerFSM->ChangeState(TEXT("LowerLayer"), TEXT("Walk")))
                m_fMoveSpeed = P_WALK_SPEED;
        }
        else
        {
            if (CPlayerStateMachine::MOVE_ACTION::CROUCH == State.eMove_State)
            {
                if (m_pPlayerFSM->ChangeState(TEXT("LowerLayer"), TEXT("Walk")))
                    m_fMoveSpeed = P_WALK_SPEED;
            }
            else
            {
                if (m_pPlayerFSM->ChangeState(TEXT("LowerLayer"), TEXT("Jog")))
                    m_fMoveSpeed = P_JOG_SPEED;
            }
        }
    }
   

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
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();

    _bool   KeyInput = false;
    _vector MovePos{}, vDir{}, vPos{}, vPlayerLook{};

    if (!State.bIsAiming)
    {
        _float3 PlayerPos = m_pTransformCom->GetPosition();
        vPos = XMLoadFloat3(&PlayerPos);
    }

    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_W))
    {
        if (State.bIsAiming)
        {
            vDir = m_pTransformCom->GetLookVector();
            m_pPlayerFSM->SetDireaction(DIRECTION::FRONT);
        }
        else
        {
            _vector CameraLook = m_pGameInstance->GetCameraState(WORLDSTATE::LOOK);
            CameraLook.m128_f32[1] = 0.f;

            vDir += CameraLook;
            m_pPlayerFSM->SetDireaction(DIRECTION::FRONT);
        }
        KeyInput = true;
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_S))
    {
        if (State.bIsAiming)
        {
            vDir = m_pTransformCom->GetLookVector() * -1.f;
            m_pPlayerFSM->SetDireaction(DIRECTION::BACK);
        }
        else
        {
            _vector CameraLook = m_pGameInstance->GetCameraState(WORLDSTATE::LOOK);
            CameraLook.m128_f32[1] = 0.f;

            vDir += CameraLook * -1.f;
            m_pPlayerFSM->SetDireaction(DIRECTION::FRONT);
        }
        KeyInput = true;
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_A))
    {
        if (State.bIsAiming)
        {
            vDir = m_pTransformCom->GetRightVector() * -1.f;
            m_pPlayerFSM->SetDireaction(DIRECTION::LEFT);
        }
        else
        {
            vDir += m_pGameInstance->GetCameraState(WORLDSTATE::RIGHT) * -1.f;
            m_pPlayerFSM->SetDireaction(DIRECTION::FRONT);
        }
        KeyInput = true;
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_D))
    {
        if (State.bIsAiming)
        {
            vDir = m_pTransformCom->GetRightVector();
            m_pPlayerFSM->SetDireaction(DIRECTION::RIGHT);
        }
        else
        {
            vDir += m_pGameInstance->GetCameraState(WORLDSTATE::RIGHT);
            m_pPlayerFSM->SetDireaction(DIRECTION::FRONT);
        }
        KeyInput = true;
    }
    if (KeyInput)
    {
        if (!State.bIsAiming)
        {
            if (!XMVector4Equal(vDir, XMVectorZero()))
                m_pTransformCom->LookAt(vPos + vDir);
        }

        MovePos = vDir * m_fMoveSpeed * fDeletaTime;
    }

    if(!XMVector3Equal(MovePos, XMVectorZero()))
        m_pTransformCom->ADD_Position(MovePos);
    XMStoreFloat3(&m_PreMovePos, MovePos);
}

void CPlayer::PlayerMoveView(_float fDeletaTime)
{
    // 이건 이제 조준상태가 아니라면 이런식으로 하고 
    
    LONG MoveMouseX = m_pGameInstance->GetMouseAxis(0);
    LONG MoveMouseY = m_pGameInstance->GetMouseAxis(1);

    if (-10 > MoveMouseX)
    {
        m_pPlayerCamera->ADDRevolutionMatrix(-10.f);
    }
    else if (10 < MoveMouseX)
    {
        m_pPlayerCamera->ADDRevolutionMatrix(10.f);
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
            m_pPlayerFSM->ChangeState(TEXT("UpperLayer"), TEXT("Default"));
        else
            m_pPlayerFSM->ChangeState(TEXT("UpperLayer"), TEXT("Crouch"));
    }

    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_RIGHT))
    {
        m_fJumpSpeed += 1.f;
    }

    if (CPlayerStateMachine::MOVE_ACTION::CROUCH >= State.eMove_State)
    {
        if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_SPACE))
        {
            if (CPlayerStateMachine::MOVE_ACTION::DEFAULT == State.eMove_State)
            {
                m_pPlayerFSM->ChangeState(TEXT("UpperLayer"), TEXT("Jump"));
                auto vPos = m_pTransformCom->GetPosition();
                m_fLandingPointY = vPos.y;
                m_fAccTime = 0.f;
                m_bIsAnimLoop = false;
            }
        }

        if (m_pGameInstance->KeyPressed(KEY_INPUT::MOUSE, 1))
            m_pPlayerFSM->SetAiming(true);
        else if (m_pGameInstance->KeyUp(KEY_INPUT::MOUSE, 1))
            m_pPlayerFSM->SetAiming(false);

        if (State.bIsAttacking)
        {
            if (IsFinishedAnimationAction())
            {
                m_pPlayerFSM->SetAttack(false);
                m_bIsAnimLoop = true;
            }
        }

        if (m_pGameInstance->KeyPressed(KEY_INPUT::MOUSE, 0))
        {
            if (!State.bIsAttacking)
            {
                m_pPlayerFSM->SetAttack(true);
                m_bIsAnimLoop = false;
            }
        }
    }
}

void CPlayer::ChangeWeapon()
{









}

HRESULT CPlayer::ADD_PlayerStateMachine()
{
    //여기서 플레이어가 데이터를 읽어서 이전에 데이터를 저장했다면
    //데이터를 세팅해준다.
    m_pPlayerFSM = CPlayerStateMachine::Create();
    if (nullptr == m_pPlayerFSM)
        return E_FAIL;

    CPlayerStateMachine::FSM_DESC Desc;
    Desc.pOwner = this;
    Desc.iLayerSize = 2;

    if (FAILED(m_pPlayerFSM->Initialize(&Desc)))
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

    Desc.vEye = { 1.f, 8.f, -6.f };
    Desc.vAt = { 1.f, 8.f, 1.f };

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

void CPlayer::UpdatePlayerAction(_float fDeletaTime)
{
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();
    if (CPlayerStateMachine::MOVE_ACTION::JUMP == State.eMove_State)
    {
        _uInt i = m_pPlayerFSM->GetStatePhase(TEXT("UpperLayer"));
        if (i < ENUM_CLASS(CState::ACTION_PHASE::END))
            UpdateJump(fDeletaTime);

        if (IsFinishedAnimationAction())
        {
            switch (i)
            {
            case 0:
                i = m_pPlayerFSM->NextStatePhase(TEXT("UpperLayer"));
                m_bIsAnimLoop = true;
            break;
            case 3 :
                i = m_pPlayerFSM->NextStatePhase(TEXT("UpperLayer"));
                m_pPlayerFSM->ChangeState(TEXT("UpperLayer"), TEXT("Default"));
                m_bIsAnimLoop = true;
            break;
            }
        }
    }
}

void CPlayer::UpdateJump(_float fDeletaTime)
{
    m_fAccTime += fDeletaTime;
    _float Velocity = m_fJumpSpeed - (0.5f * GRAVITY) * m_fAccTime * m_fAccTime;
    if ( 0.f >= Velocity && ENUM_CLASS(CState::ACTION_PHASE::STARTLOOP) == m_pPlayerFSM->GetStatePhase(TEXT("UpperLayer")))
        _uInt index =  m_pPlayerFSM->NextStatePhase(TEXT("UpperLayer"));

    ADDPosition(XMVectorSet(0.f, Velocity, 0.f, 0.f));
    auto vPos = m_pTransformCom->GetPosition();
    if (vPos.y <= m_fLandingPointY)
    {
        m_pPlayerFSM->NextStatePhase(TEXT("UpperLayer"));
        vPos.y = m_fLandingPointY;
        m_pTransformCom->SetPosition(vPos);
        m_bIsAnimLoop = false; 
    }
}

_bool CPlayer::IsFinishedAnimationAction()
{
    auto Animator = dynamic_cast<CPlayerPartData*>(FindPartObject(TEXT("Player_Animator")));
    if (Animator)
        return  Animator->IsAnimFinished();

    return false;
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
