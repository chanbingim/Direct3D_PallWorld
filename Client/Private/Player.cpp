#include "Player.h"

#include "GameInstance.h"

#include "PlayerStateMachine.h"
#include "State.h"

#include "PlayerPartData.h"
#include "PlayerWeaponSlot.h"
#include "PlayerCamera.h"

#include "PlayerManager.h"
#include "ItemBase.h"

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

    m_pPlayerCamera->SetChangeCameraMode(CPlayerCamera::CAMERA_MODE::NONE_AIMMING);
    auto PlayerManager = CPlayerManager::GetInstance();
    if (PlayerManager)
        PlayerManager->BindPlayerCharacter(this);

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
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();
    m_pAnimator->SetAnimIndex(m_pAnimator->GetAnimIndex(m_pPlayerFSM->GetStateFullName().c_str()), m_bIsAnimLoop);
    
    if (CPlayerStateMachine::MOVE_CHILD_ACTION::IDLE != State.eMove_Child_State)
    {
        if (State.bIsAiming)
            m_pAnimator->SetUppderAnimation(m_pAnimator->GetAnimIndex(m_pPlayerFSM->GetLayerAimStateName().c_str()), true);
        else 
            m_pAnimator->SetUppderAnimation(-1, false);
    }
    else
        m_pAnimator->SetUppderAnimation(-1, false);

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
            if (!State.bIsAiming)
            {
                if (m_pPlayerFSM->ChangeState(TEXT("LowerLayer"), TEXT("Sprint")))
                    m_fMoveSpeed = P_RUN_SPEED;
            }
        }
        else if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_LCONTROL))
        {
            if (!State.bIsAiming)
            {
                if (m_pPlayerFSM->ChangeState(TEXT("LowerLayer"), TEXT("Walk")))
                    m_fMoveSpeed = P_WALK_SPEED;
            }
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
   

    if (CPlayerStateMachine::NONE_COBAT_ACTION::END == State.eNone_Combat_State)
    {
        PlayerMoveView(fDeletaTime);
        ChangeAction(fDeletaTime);
        MoveAction(fDeletaTime);
        ChangeWeapon();
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
            {
                m_pTransformCom->LerpTurn(XMVectorSet(0.f, 1.f, 0.f, 0.f), vPos + vDir, 5.f, fDeletaTime);
            }
        }

        MovePos = vDir * m_fMoveSpeed * fDeletaTime;
    }

    if (!XMVector3Equal(MovePos, XMVectorZero()))
    {
        m_pTransformCom->ADD_Position(MovePos);
        if (State.bIsAttacking && !State.bIsAiming)
        {
            m_pPlayerFSM->SetAttack(false);
            m_bIsAnimLoop = true;
        }
    }
    XMStoreFloat3(&m_PreMovePos, MovePos);
}

void CPlayer::PlayerMoveView(_float fDeletaTime)
{
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();
    // 이건 이제 조준상태가 아니라면 이런식으로 하고 
    LONG MoveMouseX = m_pGameInstance->GetMouseAxis(0);
    LONG MoveMouseY = m_pGameInstance->GetMouseAxis(1);

    if (-10 > MoveMouseX)
    {
        if (State.bIsAiming)
            m_pTransformCom->Turn(m_pTransformCom->GetUpVector(), fDeletaTime, XMConvertToRadians(-90.f));
        else
            m_pPlayerCamera->ADDRevolutionRotation(-90.f, fDeletaTime);
    }
    else if (10 < MoveMouseX)
    {
        if(State.bIsAiming)
            m_pTransformCom->Turn(m_pTransformCom->GetUpVector(), fDeletaTime, XMConvertToRadians(90.f));
        else
            m_pPlayerCamera->ADDRevolutionRotation(90.f, fDeletaTime);
    }

    if (-10 > MoveMouseY)
        m_pPlayerCamera->ADDPitchRotation(-90.f, fDeletaTime);
    else if (10 < MoveMouseY)
        m_pPlayerCamera->ADDPitchRotation(90.f, fDeletaTime);
       
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

    if (CPlayerStateMachine::MOVE_ACTION::CROUCH >= State.eMove_State)
    {
#pragma region JUMP_INPUT
        if (!State.bIsAttacking)
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
        }
        
#pragma endregion 

#pragma region AIMING_INPUT
        if (ENUM_CLASS(WEAPON::NONE) != State.iWeaponType)
        {
            if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 1))
            {
                CameraDirLookAt();
                m_pPlayerCamera->SetChangeCameraMode(CPlayerCamera::CAMERA_MODE::AIMING);
                m_pPlayerFSM->SetAiming(true);
            }

            else if (m_pGameInstance->KeyUp(KEY_INPUT::MOUSE, 1))
            {
                m_pPlayerFSM->SetAiming(false);
                m_pPlayerCamera->SetChangeCameraMode(CPlayerCamera::CAMERA_MODE::NONE_AIMMING);
            }
        }
        else
            m_pPlayerFSM->SetAiming(false);
#pragma endregion

#pragma region PLAYER_ATTACK
        _bool IsAttack = false;
        if (IsFinishedAnimationAction())
        {
            if (State.bIsAttacking)
            {
                m_pAnimator->ChangeWeaponState(ENUM_CLASS(CPlayerWeaponSlot::WEAPON_STATE::IDLE));
                m_pPlayerFSM->SetAttack(false);
                m_bIsAnimLoop = true;
            }
            else
            {
                if (m_pGameInstance->KeyPressed(KEY_INPUT::MOUSE, 0) && !m_bIsAnimLoop)
                {
                    m_pPlayerFSM->NextStatePhase(TEXT("CombatLayer"));
                    m_bIsAnimLoop = true;
                }
            }
        }

        if (GetWeaponAttackType())
        {
            if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 0))
            {
                if (!State.bIsAttacking)
                {
                    m_pPlayerFSM->ChangeState(TEXT("CombatLayer"), TEXT("Attack"));
                    m_pAnimator->ChangeWeaponState(ENUM_CLASS(CPlayerWeaponSlot::WEAPON_STATE::CHARGE));
                    m_bIsAnimLoop = false;
                }
            }
            else if(m_pGameInstance->KeyUp(KEY_INPUT::MOUSE, 0))
            {
                if (!State.bIsAttacking)
                    IsAttack = true;
            }
        }
        else
        {
            if (m_pGameInstance->KeyPressed(KEY_INPUT::MOUSE, 0))
            {
                if (!State.bIsAttacking)
                    IsAttack = true;
            }
        }

        if (IsAttack)
        {
            m_pPlayerFSM->PlayerStateReset(TEXT("CombatLayer"));
            m_pPlayerFSM->SetAttack(true);
            m_pAnimator->ChangeWeaponState(ENUM_CLASS(CPlayerWeaponSlot::WEAPON_STATE::ATTACK));
            m_bIsAnimLoop = false;
        }
#pragma endregion
    }
}

void CPlayer::ChangeWeapon()
{
    _bool ItemSelect = false;
    if (0 < m_pGameInstance->GetMouseAxis(2))
    {
        CPlayerManager::GetInstance()->SwapEquipmentSlot(1);
        ItemSelect = true;
    }
    else if (0 > m_pGameInstance->GetMouseAxis(2))
    {
        CPlayerManager::GetInstance()->SwapEquipmentSlot(-1);
        ItemSelect = true;
    }

    if (ItemSelect)
    {
        const CItemBase* pItem = CPlayerManager::GetInstance()->GetSelectItemData();
        if (pItem)
        {
            ITEM_DESC ItemData = pItem->GetItemData();
            if (ITEM_TYPE::EQUIPMENT == ItemData.ItemType)
            {
                if (EUQIP_TYPE::WEAPON == ItemData.TypeDesc.EuqipDesc.Equip_Type)
                    m_pPlayerFSM->SetWeapon(ENUM_CLASS(ItemData.TypeDesc.EuqipDesc.Weapon_Type));
            }
        }
        else
        {
            m_pPlayerFSM->SetWeapon(ENUM_CLASS(WEAPON::NONE));
            m_pPlayerFSM->SetAiming(false);
            m_pPlayerCamera->SetChangeCameraMode(CPlayerCamera::CAMERA_MODE::NONE_AIMMING);
        }
         
      
    }
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

    m_pAnimator = static_cast<CPlayerPartData*>(FindPartObject(TEXT("Player_Animator")));
    
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
    if (vPos.y < m_fLandingPointY)
    {
        m_pPlayerFSM->NextStatePhase(TEXT("UpperLayer"));
        vPos.y = m_fLandingPointY;
        m_pTransformCom->SetPosition(vPos);
        m_bIsAnimLoop = false; 
    }
}

void CPlayer::CameraDirLookAt()
{
    _vector vCameraLook = m_pGameInstance->GetCameraState(WORLDSTATE::LOOK);
    _float3 PlayerPoistion = m_pTransformCom->GetPosition();
    _vector vPos = XMLoadFloat3(&PlayerPoistion);
    vCameraLook.m128_f32[1] = 0.f;

    m_pTransformCom->LookAt(vPos + vCameraLook);
}

_bool CPlayer::GetWeaponAttackType()
{
    const CItemBase* pItemData = CPlayerManager::GetInstance()->GetSelectItemData();
    if (nullptr == pItemData)
        return false;
    const ITEM_DESC ItemDesc = pItemData->GetItemData();

    if (ITEM_TYPE::EQUIPMENT == ItemDesc.ItemType)
        return ItemDesc.TypeDesc.EuqipDesc.bIsChargeAble;

    return false;
}

_bool CPlayer::IsFinishedAnimationAction()
{
    auto Animator = dynamic_cast<CPlayerPartData*>(FindPartObject(TEXT("Player_Animator")));
    if (Animator)
        return  Animator->IsAnimFinished();

    return false;
}

_bool CPlayer::IsAimingState() const
{
    if (nullptr == m_pPlayerFSM)
        return false;

    return m_pPlayerFSM->GetState().bIsAiming;
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
