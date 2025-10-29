#include "PreivewModelBody.h"

#include "GameInstance.h"
#include "PlayerBody.h"
#include "PreviewModelWeapon.h"

CPreivewModelBody::CPreivewModelBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CPreivewModelBody::CPreivewModelBody(const CPreivewModelBody& rhs) :
    CPartObject(rhs)
{
}

HRESULT CPreivewModelBody::Initalize_Prototype()
{

    return S_OK;
}

HRESULT CPreivewModelBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_ChildObject()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CPreivewModelBody::Priority_Update(_float fDeletaTime)
{
    m_pWeaponSocket->Priority_Update(fDeletaTime);
    m_pPlayerBody->Priority_Update(fDeletaTime);
}

void CPreivewModelBody::Update(_float fDeletaTime)
{
    m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime, 10.f, m_bIsAnimLoop);
    m_pPlayerBody->UpdateAnimation(m_pVIBufferCom);

    m_pPlayerBody->Update(fDeletaTime);
    m_pWeaponSocket->Update(fDeletaTime);
}

void CPreivewModelBody::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();

    m_pWeaponSocket->Late_Update(fDeletaTime);
    m_pPlayerBody->Late_Update(fDeletaTime);
}

HRESULT CPreivewModelBody::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(2);

        //어떤 파츠데이터를 사용하여 랜더링하는지를 알려준다.
        // 0 머리
        // 1 얼굴
        // 2 몸통 등 이런순서로 랜더링을 걸어가면서 하면 될거같음
        if (i == iNumMeshes - 1)
            m_pPlayerBody->PreviewRender();
        else
            m_pVIBufferCom->Render(i);

    }
    m_pWeaponSocket->Render();
    return S_OK;
}

void CPreivewModelBody::SetUppderAnimation(_uInt iIndex, _bool bIsSplite)
{
}

void CPreivewModelBody::ChangeSocketFlag(_char bitFlag)
{
}

void CPreivewModelBody::ChangeWeaponState(_uInt iWeaponState, _bool bIsAnimLoop)
{
}

const _float4x4* CPreivewModelBody::GetLeftHandSocket()
{
    return nullptr;
}

HRESULT CPreivewModelBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Player_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CPreivewModelBody::ADD_ChildObject()
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

    CPreviewModelWeapon::WEAPON_SLOT_DESC SlotDesc;
#pragma region Slot 1
    SlotDesc.pParent = m_pParent;
    lstrcpy(SlotDesc.ObjectTag, L"Player_R_Weapon");
    SlotDesc.vScale = { 1.f, 1.f, 1.f };
    SlotDesc.vRotation = { XMConvertToRadians(90.f), 0.f,  XMConvertToRadians(180.f) };
    SlotDesc.vPosition = { 0.01f, 0.f, 0.f };
    SlotDesc.UseSocketMatrixFlag = 0b00000001;
    m_pWeaponSocketMatrix = GetBoneMatrix("weapon_r");
    SlotDesc.SocketMatrix = m_pWeaponSocketMatrix;

    m_pLeftHandSocket = GetBoneMatrix("weapon_l");
    SlotDesc.pLeftSocket = m_pLeftHandSocket;
    SlotDesc.iSlotIndex = 0;

    m_pWeaponSocket = static_cast<CPreviewModelWeapon*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PreviewModelWeapon"), &SlotDesc));
    __super::ADD_Child(m_pWeaponSocket);
#pragma endregion

    return S_OK;
}

CPreivewModelBody* CPreivewModelBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPreivewModelBody* pPreviewModelBody = new CPreivewModelBody(pDevice, pContext);
    if (FAILED(pPreviewModelBody->Initalize_Prototype()))
    {
        Safe_Release(pPreviewModelBody);
        MSG_BOX("CREATE FAIL : PREVIEW MODEL BODY");
    }
    return pPreviewModelBody;
}

CGameObject* CPreivewModelBody::Clone(void* pArg)
{
    CPreivewModelBody* pPreviewModelBody = new CPreivewModelBody(*this);
    if (FAILED(pPreviewModelBody->Initialize(pArg)))
    {
        Safe_Release(pPreviewModelBody);
        MSG_BOX("CLONE FAIL : PREVIEW MODEL BODY");
    }
    return pPreviewModelBody;
}

void CPreivewModelBody::Free()
{
    __super::Free();

    Safe_Release(m_pPlayerBody);
    Safe_Release(m_pWeaponSocket);
}
