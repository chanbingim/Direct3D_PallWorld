#include "BedCatBody.h"

#include "GameInstance.h"
#include "PellSkillManager.h"

#include "PellBase.h"
#include "HitBox.h"

CBedCatBody::CBedCatBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPellBody(pDevice, pContext)
{
}

CBedCatBody::CBedCatBody(const CBedCatBody& rhs) :
    CPellBody(rhs)
{
}

HRESULT CBedCatBody::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBedCatBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Insert_KeyFrame()))
        return E_FAIL;


    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pNoiseTexture = m_pGameInstance->GetTextureResource(TEXT("T_Default_Noise.png"));

    return S_OK;
}

void CBedCatBody::Priority_Update(_float fDeletaTime)
{
}

void CBedCatBody::Update(_float fDeletaTime)
{
    m_bIsAnimFinished = m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime, 10.f , m_bIsAnimLoop);
}

void CBedCatBody::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);

    for (auto pCollision : m_pSkillHitBox)
        pCollision->Late_Update(fDeletaTime);
}

HRESULT CBedCatBody::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);
        m_pShaderCom->Bind_RawValue("g_bIsDissolve", &m_bIsDissolve, sizeof(_bool));
        if(m_bIsDissolve)
        {
            m_pShaderCom->Bind_SRV("g_DissolveTexture", m_pNoiseTexture->GetTexture(0));
            m_pShaderCom->Bind_RawValue("g_fDissloveTime", &m_fAccDissolveTime, sizeof(_float));
        }

        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render(i);
    }

    return S_OK;
}

HRESULT CBedCatBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_BedCat_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    CHitBox::HIT_BOX_DESC HitBoxDesc = {};
    HitBoxDesc.pParent = this;
    HitBoxDesc.pOwner = m_pParent;
    HitBoxDesc.pOwnerWorldMat = &m_CombinedWorldMatrix;

    HitBoxDesc.SocketMatrix = GetBoneMatrix("weapon_l");
    HitBoxDesc.vScale = { 0.1f, 0.1f, 0.1f };
    m_pSkillHitBox[0] = static_cast<CHitBox*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Socket_HitBox"), &HitBoxDesc));
    if (nullptr == m_pSkillHitBox[0])
        return E_FAIL;
    
    HitBoxDesc.SocketMatrix = GetBoneMatrix("weapon_r");
    HitBoxDesc.vScale = { 0.1f, 0.1f, 0.1f };
    m_pSkillHitBox[1] = static_cast<CHitBox*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Socket_HitBox"), &HitBoxDesc));
    if(nullptr == m_pSkillHitBox[1])
        return E_FAIL;

    return S_OK;
}

HRESULT CBedCatBody::Insert_KeyFrame()
{
    m_pVIBufferCom->Bind_KeyFrameFunction("NekoPunch_Loop", 2, [&]() { AttackOnCollisionR(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("NekoPunch_Loop", 10, [&]() { AttackOnCollisionL(); });

    return S_OK;
}

void CBedCatBody::AttackOnCollisionR()
{
    auto pSkillData = CPellSkillManager::GetInstance()->FindPellData(m_iAttackSkillIndex);
    m_pSkillHitBox[1]->StartAttack(pSkillData->iSkillDamage);
}

void CBedCatBody::UpateCollisionR()
{
    auto pSkillData = CPellSkillManager::GetInstance()->FindPellData(m_iAttackSkillIndex);
    m_pSkillHitBox[1]->UpdateAttack(pSkillData->iSkillDamage);
}

void CBedCatBody::AttackOnCollisionL()
{
    auto pSkillData = CPellSkillManager::GetInstance()->FindPellData(m_iAttackSkillIndex);
    m_pSkillHitBox[0]->StartAttack(pSkillData->iSkillDamage);
}

void CBedCatBody::UpateCollisionL()
{
    auto pSkillData = CPellSkillManager::GetInstance()->FindPellData(m_iAttackSkillIndex);
    m_pSkillHitBox[0]->UpdateAttack(pSkillData->iSkillDamage);
}

CBedCatBody* CBedCatBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBedCatBody* pBedCatBody = new CBedCatBody(pDevice, pContext);
    if (FAILED(pBedCatBody->Initalize_Prototype()))
    {
        Safe_Release(pBedCatBody);
        MSG_BOX("CREATE FAIL : BED CAT BODY");
    }
    return pBedCatBody;
}

CGameObject* CBedCatBody::Clone(void* pArg)
{
    CBedCatBody* pBedCatBody = new CBedCatBody(*this);
    if (FAILED(pBedCatBody->Initialize(pArg)))
    {
        Safe_Release(pBedCatBody);
        MSG_BOX("CLONE FAIL : BED CAT BODY");
    }
    return pBedCatBody;
}

void CBedCatBody::Free()
{
    __super::Free();

    for(auto pHitBox  : m_pSkillHitBox)
        Safe_Release(pHitBox);
}
