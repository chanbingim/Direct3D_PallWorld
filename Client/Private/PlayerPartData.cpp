#include "PlayerPartData.h"

#include "GameInstance.h"
#include "PlayerCamera.h"

#include "PlayerBody.h"
#include "PlayerWeapon.h"

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

    if (FAILED(ADD_ChildObject()))
        return E_FAIL;

    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;



    return S_OK;
}

void CPlayerPartData::Priority_Update(_float fDeletaTime)
{

}

void CPlayerPartData::Update(_float fDeletaTime)
{
    m_bIsFinished = m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime, 5.f, m_bIsAnimLoop);
}

void CPlayerPartData::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();

    //행렬가지고 갱신
    m_pPlayerBody->Late_Update(fDeletaTime);
    m_pPlayerWeapon->Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayerPartData::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

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
    m_pWeaponSocketMatrix[1] = GetBoneMatrix("upperarm_l");
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
    Desc.vScale = { 0.05f, 0.05f, 0.05f };
    Desc.SocketMatrix = &m_CombinedWorldMatrix;
    Desc.UseSocketMatrixFlag = 0b00000001;
    Desc.vRotation = { 0.f, 0.f, 0.f };
    lstrcpy(Desc.ObjectTag, L"BodyPart");

    m_pPlayerBody = static_cast<CPlayerBody*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Body_Default"), &Desc));
    __super::ADD_Child(m_pPlayerBody);

    Desc.pParent = m_pParent;
    lstrcpy(Desc.ObjectTag, L"Player_R_Weapon");
    Desc.vScale = { 1.f, 1.f, 1.f };
    Desc.vRotation = { XMConvertToRadians(0.f),  XMConvertToRadians(0.f),  XMConvertToRadians(-90.f) };
    Desc.vPosition = {-10.f, 10.f, 0.f};
    Desc.UseSocketMatrixFlag = 0b00000001;
    m_pWeaponSocketMatrix[0] = GetBoneMatrix("weapon_r");
    //Desc.SocketMatrix = m_pPlayerBody->GetBoneMatrix("upperarm_l");
    Desc.SocketMatrix = m_pWeaponSocketMatrix[1];

    m_pPlayerWeapon = static_cast<CPlayerWeapon*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Weapon"), &Desc));
    __super::ADD_Child(m_pPlayerWeapon);
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
    Safe_Release(m_pPlayerWeapon);
}
