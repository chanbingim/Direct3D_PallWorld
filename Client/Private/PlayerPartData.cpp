#include "PlayerPartData.h"

#include "GameInstance.h"
#include "PlayerCamera.h"

#include "PlayerBody.h"
#include "PlayerWeaponSlot.h"

#include "PlayerManager.h"
#include "ItemBase.h"

CPlayerPartData::CPlayerPartData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CPlayerPartData::CPlayerPartData(const CPlayerPartData& rhs) :
    CPartObject(rhs)
{
}

HRESULT CPlayerPartData::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerPartData::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Insert_AnimKeyFrameFunction()))
        return E_FAIL;

    if (FAILED(ADD_ChildObject()))
        return E_FAIL;

    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    m_pSpineOffsetMatrix = m_pVIBufferCom->GetTransformationOffsetMatrixPtr("spine_01");
    return S_OK;
}

void CPlayerPartData::Priority_Update(_float fDeletaTime)
{
    for (_uInt i = 0; i < 2; ++i)
        m_pWeaponSocket[i]->Priority_Update(fDeletaTime);
}

void CPlayerPartData::Update(_float fDeletaTime)
{
    if(!m_SplitAnimation)
        m_bIsFinished = m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime, 10.f, m_bIsAnimLoop);
    else
    {
        // 여기서 특정 상태일때 애니메이션을 또하나 재생한다.
        // 애니메이션을 두개 재생하고 바디에게 넘겨줄때 바디는 두개의 데이터를 받아서
        // 상체 하체를 분할해서 애니메이션을 재생한다.
        m_pVIBufferCom->PlayAnimation(1, m_iAnimIndex, fDeletaTime, m_fAnimSpeed, true);
        m_bIsFinished = m_pVIBufferCom->PlayAnimation(0, m_UpperBodyIndex, fDeletaTime, 10.f, m_bIsAnimLoop, "spine_02");
    }

    m_pPlayerBody->UpdateAnimation(m_pVIBufferCom);
    for (_uInt i = 0; i < 2; ++i)
        m_pWeaponSocket[i]->Update(fDeletaTime);
}

void CPlayerPartData::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();

    //행렬가지고 갱신
    m_pPlayerBody->Late_Update(fDeletaTime);

    for (_uInt i = 0; i < 2; ++i)
        m_pWeaponSocket[i]->Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    m_pGameInstance->Add_RenderGroup(RENDER::SHADOW, this);
}

HRESULT CPlayerPartData::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);
        m_pShaderCom->Bind_RawValue("g_bIsDissolve", &m_bIsDissolve, sizeof(_bool));

        m_pShaderCom->Update_Shader(0);
        //어떤 파츠데이터를 사용하여 랜더링하는지를 알려준다.
        // 0 머리
        // 1 얼굴
        // 2 몸통 등 이런순서로 랜더링을 걸어가면서 하면 될거같음
        if (i == iNumMeshes - 1)
            m_pPlayerBody->Render();
        else
            m_pVIBufferCom->Render(i);

    }
    return S_OK;
}

HRESULT CPlayerPartData::ShadowRender()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);
        Apply_ShadowShaderResources();
        m_pShaderCom->Update_Shader(1);

        //어떤 파츠데이터를 사용하여 랜더링하는지를 알려준다.
        // 0 머리
        // 1 얼굴
        // 2 몸통 등 이런순서로 랜더링을 걸어가면서 하면 될거같음
        if (i == iNumMeshes - 1)
            m_pPlayerBody->ShadowRender();
        else
            m_pVIBufferCom->Render(i);

    }
    return S_OK;
}

void CPlayerPartData::SetUppderAnimation(_uInt iIndex, _bool bIsSplite)
{
    m_UpperBodyIndex = iIndex;
    m_SplitAnimation = bIsSplite;
}

void CPlayerPartData::ChangeSocketFlag(_char bitFlag)
{
    //SetSocketFlag(bitFlag);
    m_pPlayerBody->SetSocketFlag(bitFlag);
}

void CPlayerPartData::ChangeWeaponState(_uInt iWeaponState, _bool bIsAnimLoop)
{
    m_pWeaponSocket[0]->ChangeWeaponState(CPlayerWeaponSlot::WEAPON_STATE(iWeaponState), bIsAnimLoop);
}

void CPlayerPartData::StartAttackSlot()
{
    m_pGameInstance->Manager_PlaySound(TEXT("SwordSound.wav"), CHANNELID::EFFECT2, 1.f);
    static_cast<CPlayerWeaponSlot*>(m_pWeaponSocket[0])->StartAttack();
}

void CPlayerPartData::UpdateAttackSlot()
{
    static_cast<CPlayerWeaponSlot*>(m_pWeaponSocket[0])->UpdateAttack();
}

void CPlayerPartData::RoatationPitchSpine(_float fPitchAngle)
{
    XMStoreFloat4x4(m_pSpineOffsetMatrix, 
        XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(0.f, 0.f, -fPitchAngle)));
}

void CPlayerPartData::ResetWeaponSlot(_uInt iIndex)
{
    m_pWeaponSocket[iIndex]->ResetVIBuffer();
}

const _float4x4* CPlayerPartData::GetLeftHandSocket()
{
    return m_pLeftHandSocket;
}

HRESULT CPlayerPartData::ShootProjecttileObject()
{
    return static_cast<CPlayerWeaponSlot *>(m_pWeaponSocket[0])->ShootProjecttileObject();
}

void CPlayerPartData::WalkEvent()
{
    m_pGameInstance->Manager_StopSound(CHANNELID::EFFECT2);
    m_pGameInstance->Manager_PlaySound(TEXT("Walk.wav"), CHANNELID::EFFECT2, 1.f);
}

HRESULT CPlayerPartData::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Player_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerPartData::ADD_ChildObject()
{
    m_pWeaponSocketMatrix[2] = GetBoneMatrix("upperarm_r");

    if (FAILED(ADD_AnimParts()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerPartData::ADD_AnimParts()
{
    CPartObject::PARTOBJECT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CPartObject::PARTOBJECT_DESC));

    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };
    Desc.SocketMatrix = &m_CombinedWorldMatrix;
    Desc.UseSocketMatrixFlag = 0b00000001;
    Desc.vRotation = { 0.f, 0.f, 0.f };
    lstrcpy(Desc.ObjectTag, L"BodyPart");

    m_pPlayerBody = static_cast<CPlayerBody*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Body_Default"), &Desc));
    __super::ADD_Child(m_pPlayerBody);

    CPlayerWeaponSlot::WEAPON_SLOT_DESC SlotDesc;
#pragma region Slot 1
    SlotDesc.pParent = m_pParent;
    lstrcpy(SlotDesc.ObjectTag, L"Player_R_Weapon");
    SlotDesc.vScale = { 1.f, 1.f, 1.f };
    SlotDesc.vRotation = { XMConvertToRadians(90.f), 0.f,  XMConvertToRadians(180.f) };
    SlotDesc.vPosition = { 0.01f, 0.f, 0.f };
    SlotDesc.UseSocketMatrixFlag = 0b00000001;
    m_pWeaponSocketMatrix[0] = GetBoneMatrix("weapon_r");
    SlotDesc.SocketMatrix = m_pWeaponSocketMatrix[0];

    m_pLeftHandSocket = GetBoneMatrix("weapon_l");
    SlotDesc.pLeftSocket = m_pLeftHandSocket;
    SlotDesc.iSlotIndex = 0;

    m_pWeaponSocket[0] = static_cast<CPlayerWeaponSlot*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_WeaponSot"), &SlotDesc));
    __super::ADD_Child(m_pWeaponSocket[0]);
#pragma endregion

#pragma region Slot 2
    CPlayerItemSlot::ITEM_SLOT_DESC ItemSlotDesc = {};
    ItemSlotDesc.pParent = m_pParent;
    lstrcpy(ItemSlotDesc.ObjectTag, L"Player_L_Socket");
    ItemSlotDesc.vScale = { 1.f, 1.f, 1.f };
    ItemSlotDesc.vRotation = { 0.f,  0.f,  XMConvertToRadians(-90.f) };
    ItemSlotDesc.UseSocketMatrixFlag = 0b00000001;
    ItemSlotDesc.vPosition = { -0.1f, 0.1f, 0.f };
    ItemSlotDesc.SocketMatrix = GetBoneMatrix("upperarm_l");
    ItemSlotDesc.iSlotIndex = 1;

    m_pWeaponSocket[1] = static_cast<CPlayerItemSlot*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_BackItemSlot"), &ItemSlotDesc));
    __super::ADD_Child(m_pWeaponSocket[1]);
#pragma endregion

    return S_OK;
}

HRESULT CPlayerPartData::Insert_AnimKeyFrameFunction()
{
    m_pVIBufferCom->Bind_KeyFrameFunction("Attack_Bow", 1, [this]() { ShootProjecttileObject(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Attack_Throw", 4, [this]() { ShootProjecttileObject(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Attack_Melee", 17, [this]() { StartAttackSlot(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Attack_Melee", 37, [this]() { StartAttackSlot(); });

    for (_uInt i = 18; i < 20; ++i)
    {
        m_pVIBufferCom->Bind_KeyFrameFunction("Attack_Melee", i, [this]() { UpdateAttackSlot(); });
        m_pVIBufferCom->Bind_KeyFrameFunction("Attack_Melee", i + 20, [this]() { UpdateAttackSlot(); });
    }
    
    m_pVIBufferCom->Bind_KeyFrameFunction("Attack_None", 13, [this]() { StartAttackSlot(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Attack_None", 33, [this]() { StartAttackSlot(); });

    for (_uInt i = 14; i < 16; ++i)
    {
        m_pVIBufferCom->Bind_KeyFrameFunction("Attack_None", i, [this]() { UpdateAttackSlot(); });
        m_pVIBufferCom->Bind_KeyFrameFunction("Attack_None", i + 20, [this]() { UpdateAttackSlot(); });
    }
#pragma region Walk Key Frame

    m_pVIBufferCom->Bind_KeyFrameFunction("Walk_None", 21, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Walk_None", 36, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Sprint_None", 11, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Sprint_None", 16, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_None", 15, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_None", 23, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Walk", 11, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Walk", 19, [&]() { WalkEvent(); });

    // Weapon 무기
    m_pVIBufferCom->Bind_KeyFrameFunction("Walk_Melee", 21, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Walk_Melee", 36, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Sprint_Melee", 11, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Sprint_Melee", 16, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Melee", 15, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Melee", 23, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Melee", 11, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Melee", 19, [&]() { WalkEvent(); });

#pragma region Jog_Aim_Melee
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Aim_Melee", 21, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Aim_Melee", 36, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Right_Aim_Melee", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Right_Aim_Melee", 18, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Left_Aim_Melee", 13, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Left_Aim_Melee", 23, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Bwd_Aim_Melee", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Bwd_Aim_Melee", 21, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Aim_Melee", 21, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Aim_Melee", 36, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Right_Aim_Melee", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Right_Aim_Melee", 18, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Left_Aim_Melee", 13, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Left_Aim_Melee", 22, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Bwd_Aim_Melee", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Bwd_Aim_Melee", 21, [&]() { WalkEvent(); });
#pragma endregion

    // Throw 무기
    m_pVIBufferCom->Bind_KeyFrameFunction("Walk_Throw", 21, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Walk_Throw", 36, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Sprint_Throw", 11, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Sprint_Throw", 16, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Throw", 15, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Throw", 23, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Throw", 11, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Throw", 19, [&]() { WalkEvent(); });

#pragma region Jog_Aim_Thorw
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Aim_Throw", 21, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Aim_Throw", 36, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Right_Aim_Throw", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Right_Aim_Throw", 18, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Left_Aim_Throw", 13, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Left_Aim_Throw", 23, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Bwd_Aim_Throw", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Bwd_Aim_Throw", 22, [&]() { WalkEvent(); });

    // 앉기
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Aim_Throw", 21, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Aim_Throw", 36, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Right_Aim_Throw", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Right_Aim_Throw", 18, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Left_Aim_Throw", 13, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Left_Aim_Throw", 23, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Bwd_Aim_Throw", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Bwd_Aim_Throw", 22, [&]() { WalkEvent(); });
#pragma endregion

    // Bow 무기
    m_pVIBufferCom->Bind_KeyFrameFunction("Walk_Bow", 21, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Walk_Bow", 36, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Sprint_Bow", 11, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Sprint_Bow", 16, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Bow", 15, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Bow", 23, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Bow", 11, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Bow", 19, [&]() { WalkEvent(); });

#pragma region Jog_Aim_Bow
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Aim_Bow", 21, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Aim_Bow", 36, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Right_Aim_Bow", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Right_Aim_Bow", 18, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Left_Aim_Bow", 13, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Left_Aim_Bow", 23, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Bwd_Aim_Bow", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Jog_Bwd_Aim_Bow", 22, [&]() { WalkEvent(); });

    // 앉기
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Aim_Bow", 21, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Aim_Bow", 36, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Right_Aim_Bow", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Right_Aim_Bow", 18, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Left_Aim_Bow", 13, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Left_Aim_Bow", 23, [&]() { WalkEvent(); });

    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Bwd_Aim_Bow", 8, [&]() { WalkEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("Crouch_Bwd_Aim_Bow", 22, [&]() { WalkEvent(); });
#pragma endregion
#pragma endregion

    return S_OK;
}

CPlayerPartData* CPlayerPartData::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerPartData* pPlayerPartData = new CPlayerPartData(pDevice, pContext);
    if (FAILED(pPlayerPartData->Initalize_Prototype()))
    {
        Safe_Release(pPlayerPartData);
        MSG_BOX("CREATE FAIL : PLAYER PARTS DATA");
    }
    return pPlayerPartData;
}

CGameObject* CPlayerPartData::Clone(void* pArg)
{
    CPlayerPartData* pPlayerPartData = new CPlayerPartData(*this);
    if (FAILED(pPlayerPartData->Initialize(pArg)))
    {
        Safe_Release(pPlayerPartData);
        MSG_BOX("CLONE FAIL : PLAYER PARTS DATA");
    }
    return pPlayerPartData;
}

void CPlayerPartData::Free()
{
    __super::Free();

    Safe_Release(m_pPlayerBody);

    for (_uInt i = 0; i < 3; ++i)
        Safe_Release(m_pWeaponSocket[i]);

}
