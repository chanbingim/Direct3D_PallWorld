#include "Player.h"

#include "GameInstance.h"

#include "PlayerStateMachine.h"
#include "State.h"

#include "TerrainManager.h"
#include "PlayerPartData.h"
#include "PlayerWeaponSlot.h"
#include "PlayerCamera.h"

#include "PlayerManager.h"
#include "JumpState.h"
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

    if (FAILED(ADD_Components()))
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

    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_F10))
        m_ViewCamera = !m_ViewCamera;

    if(m_ViewCamera)
        m_pPlayerCamera->Late_Update(fDeletaTime);

    if (GAMEMODE::GAME == m_pGameInstance->GetGameMode())
    {
        Key_Input(fDeletaTime);
    }
    m_pGameInstance->SetPlayerWorldMatrix(m_pTransformCom->GetWorldMat());
}

void CPlayer::Update(_float fDeletaTime)
{
    m_pPlayerFSM->Update(fDeletaTime);
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();
    
    m_pAnimator->SetAnimIndex(m_pAnimator->GetAnimIndex(m_pPlayerFSM->GetStateFullName().c_str()), m_fAnimSpeed, m_bIsAnimLoop);
    if (CPlayerStateMachine::MOVE_CHILD_ACTION::IDLE != State.eMove_Child_State)
    {
        if (State.bIsAiming || State.bIsAttacking)
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
   
    m_pGameInstance->ADD_CollisionList(m_pCollision);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayer::Render()
{
    m_pCollision->Render();
    //m_pNevigation->Render({0.f,0.f,1.f,1.f}, true);
    
    return S_OK;
}

void CPlayer::Key_Input(_float fDeletaTime)
{
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();

    if (XMVector3Equal(XMLoadFloat3(&m_PreMovePos), XMVectorZero()))
    {
        m_pPlayerFSM->ChangeState(TEXT("LowerLayer"), TEXT("Idle"));
    }
    if (!XMVector3Equal(XMLoadFloat3(&m_PreMovePos), XMVectorZero()))
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
    if (!GetWeaponAttackType() && State.bIsAttacking)
        m_pAnimator->NearAttackOnCollision();

    MoveAction(fDeletaTime);
    PlayerMoveView(fDeletaTime);
    ChangeAction(fDeletaTime);
    ChangeWeapon();

    if (CPlayerStateMachine::MOVE_ACTION::JUMP != State.eMove_State)
        m_pNevigation->ComputeHeight(m_pTransformCom);

    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

void CPlayer::MoveAction(_float fDeletaTime)
{
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();

    _bool   KeyInput = false;
    _vector MovePos{}, vDir{}, vPos{}, vPlayerLook{};

    _float3 PlayerPos = m_pTransformCom->GetPosition();
    vPos = XMLoadFloat3(&PlayerPos);

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
                m_pTransformCom->LerpTurn(XMVectorSet(0.f, 1.f, 0.f, 0.f), vPos + vDir, XMConvertToRadians(180.f), fDeletaTime);
            }
        }

        MovePos = XMVector3Normalize(vDir) * m_fMoveSpeed * fDeletaTime;
    }

    if (!XMVector3Equal(MovePos, XMVectorZero()))
    {
        if(m_pNevigation->IsMove(vPos + MovePos))
            m_pTransformCom->ADD_Position(MovePos);

        if (State.bIsAttacking && !State.bIsAiming)
        {
            m_pPlayerFSM->ResetLayer(TEXT("CombatLayer"));
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

    _float spineRotAngle = {};
    if (State.bIsAiming)
        spineRotAngle = m_pPlayerCamera->GetPlayerCameraPitch();
    else
        spineRotAngle = 0.f;

    m_pAnimator->RoatationPitchSpine(spineRotAngle);
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
                    CJumpState::JUMPSTATE_DESC JumpStateDesc = {};
                    JumpStateDesc.fAnimSpeed = &m_fAnimSpeed;

                    m_pPlayerFSM->ChangeState(TEXT("UpperLayer"), TEXT("Jump"), &JumpStateDesc);
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
                m_bIsAnimLoop = true;
                m_pPlayerFSM->SetAttack(false);
                m_pAnimator->ChangeWeaponState(ENUM_CLASS(CPlayerWeaponSlot::WEAPON_STATE::IDLE), m_bIsAnimLoop);
            }
            else
            {
                if (m_pGameInstance->KeyPressed(KEY_INPUT::MOUSE, 0) && !m_bIsAnimLoop)
                {
                    m_bIsAnimLoop = true;
                    m_pPlayerFSM->NextStatePhase(TEXT("CombatLayer"));
                }
            }
        }

        if (GetWeaponAttackType())
        {
            if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 0))
            {
                if (!State.bIsAttacking)
                {
                    m_bIsAnimLoop = false;
                    m_pPlayerFSM->ChangeState(TEXT("CombatLayer"), TEXT("Attack"));
                    m_pAnimator->ChangeWeaponState(ENUM_CLASS(CPlayerWeaponSlot::WEAPON_STATE::CHARGE), m_bIsAnimLoop);
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
            //m_pAnimator->ShootProjecttileObject();
            m_bIsAnimLoop = false;
        }
#pragma endregion

#pragma region PELL CREATE
        if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_F))
        {
            CPlayerManager::GetInstance()->SpawnSelectPell();
        }

#pragma endregion

    }
}

void CPlayer::ChangeWeapon()
{
    CPlayerStateMachine::PLAYER_STATE State = m_pPlayerFSM->GetState();
    if (State.bIsAttacking)
        return;

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

    Desc.vEye = { 0.5f, 1.5f, -2.f };
    Desc.vAt = { 0.5f, 1.5f, 1.f };

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

HRESULT CPlayer::ADD_Components()
{
#pragma region COLLISION
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    ZeroMemory(&OBBDesc, sizeof(COBBCollision::OBB_COLLISION_DESC));
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = _float3(0.5f, 1.2f, 0.5f);
    OBBDesc.vCneter = _float3(0.f, OBBDesc.vExtents.y * 0.5f, 0.f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &OBBDesc)))
        return E_FAIL;
#pragma endregion

#pragma region NAVI_MESH
    m_pTerrainManager = CTerrainManager::GetInstance();
    auto FindNaviMesh = m_pTerrainManager->GetNavimesh();
    if (FindNaviMesh)
    {
        CNavigation::NAVIGATION_DESC Desc = {};
        _float3 vPos = m_pTransformCom->GetPosition();
        Desc.iCurrentCellIndex = 100;

        m_pTransformCom->SetPosition(FindNaviMesh->CellCenterPos(Desc.iCurrentCellIndex));
        m_pNevigation = static_cast<CNavigation*>(FindNaviMesh->Clone(&Desc));

        Safe_AddRef(m_pNevigation);
        m_pComponentMap.emplace(TEXT("NaviMesh_Com"), m_pNevigation);
    }
#pragma endregion
 
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

void CPlayer::SetPlayerData(CHARACTER_DESC* pPlayerInfo)
{
    m_pCharacterInfo = pPlayerInfo;
}

void CPlayer::GetPlayerState(void* pOut)
{
    CPlayerStateMachine::PLAYER_STATE* PlayerState = static_cast<CPlayerStateMachine::PLAYER_STATE*>(pOut);
    *PlayerState = m_pPlayerFSM->GetState();
}

void CPlayer::Damage(void* pArg, CActor* pDamagedActor)
{
    if (nullptr == pArg)
        return;

    DEFAULT_DAMAGE_DESC* pDamageDesc = static_cast<DEFAULT_DAMAGE_DESC*>(pArg);
    if (pDamageDesc)
    {
        m_pCharacterInfo->CurHealth -= pDamageDesc->fDmaged;
        if (0 >= m_pCharacterInfo->CurHealth)
        {
            //여기서 대충 페이드 아웃할거임
            m_pPlayerFSM->ChangeState(TEXT("CombatLayer"), TEXT("Dead"));
            m_bIsAnimLoop = false;
            m_pCharacterInfo->CurHealth = 0;
        }
        else
        {
            m_pPlayerFSM->ChangeState(TEXT("CombatLayer"), TEXT("Hit"));
            m_bIsAnimLoop = false;
        }
    }
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
    Safe_Release(m_pNevigation);
    Safe_Release(m_pCollision);
}
