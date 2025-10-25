#include "SkillObjectBase.h"

#include "GameInstance.h"
#include "PellSkillManager.h"
#include "EffectContatiner.h"

CSkillObjectBase::CSkillObjectBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CActor(pDevice, pContext)
{
}

CSkillObjectBase::CSkillObjectBase(const CSkillObjectBase& rhs) :
    CActor(rhs),
    m_SkillData(rhs.m_SkillData)
{
}

HRESULT CSkillObjectBase::Initalize_Prototype()
{
    auto pSkillData = CPellSkillManager::GetInstance()->FindPellData(m_iNumSkill);
    if (pSkillData)
        m_SkillData = *pSkillData;

    return S_OK;
}

HRESULT CSkillObjectBase::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    SKILL_OBJECT_DESC* pObjectDesc = static_cast<SKILL_OBJECT_DESC*>(pArg);
    m_vTargetDir = pObjectDesc->vTargetDir;
    m_pOwner = pObjectDesc->pOwner;
    return S_OK;
}

void CSkillObjectBase::Priority_Update(_float fDeletaTime)
{
    for (auto pEffect : m_pSkillEffects)
        pEffect->Priority_Update(fDeletaTime);

    if (m_SkillData.bIsProjectTile)
    {
        _vector vDir = XMLoadFloat3(&m_vTargetDir);
        _vector vMovePos = vDir * fDeletaTime * m_SkillData.fSkillMoveSpeed;

        _float3 vTargetPoint = {};
        m_pTransformCom->SetPosition(vTargetPoint);
    }

    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

void CSkillObjectBase::Update(_float fDeletaTime)
{
    for (auto pEffect : m_pSkillEffects)
        pEffect->Update(fDeletaTime);

    if (m_bIsHitTick)
    {
        m_fAccTickDamageTime += fDeletaTime;
    }

}

void CSkillObjectBase::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        for (auto pEffect : m_pSkillEffects)
            pEffect->Late_Update(fDeletaTime);

        m_pGameInstance->ADD_CollisionList(m_pCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONLIGHT, this);
    }
}

HRESULT CSkillObjectBase::Render()
{
    m_pCollision->Render();
    return S_OK;
}

void CSkillObjectBase::HitOverlapEvent(_float3 vDir, CGameObject* pHitObject)
{
    auto pHitActor = static_cast<CActor*>(pHitObject);
    DEFAULT_DAMAGE_DESC DamageDesc = {};
    DamageDesc.fDmaged = m_SkillData.iSkillDamage;

    pHitActor->Damage(&DamageDesc, this);
    if (PELL_SKILL_DAMAGE_TYPE::HIT == m_SkillData.eSkillDamageType)
        SetDead(true);
    else if (PELL_SKILL_DAMAGE_TYPE::OVERLAP == m_SkillData.eSkillDamageType)
    {
        m_fAccTickDamageTime = 0.f;
        m_fTickDamageTime = m_SkillData.fTickDamageTime;
    }
}

void CSkillObjectBase::HitOverlapping(_float3 vDir, CGameObject* pHitObject)
{
    // 여긴 틱템을 넣자
    // 회오리는 틱템으로 주자
    if (PELL_SKILL_DAMAGE_TYPE::OVERLAP == m_SkillData.eSkillDamageType)
    {
        DEFAULT_DAMAGE_DESC DamageDesc = {};
        DamageDesc.fDmaged = m_SkillData.iSkillDamage;

        if (m_fAccTickDamageTime >= m_fTickDamageTime)
        {
            auto pHitActor = static_cast<CActor*>(pHitObject);
            pHitActor->Damage(&DamageDesc, this);
            m_fAccTickDamageTime = 0.f;
        }

        m_bIsHitTick = true;
    }
}

void CSkillObjectBase::HitOverlapEnd(_float3 vDir, CGameObject* pHitObject)
{
    m_bIsHitTick = false; 
    m_fAccTickDamageTime = 0.f;

}

CGameObject* CSkillObjectBase::Clone(void* pArg)
{
    return nullptr;
}

void CSkillObjectBase::Free()
{
    __super::Free();

    Safe_Release(m_pCollision);
    for (auto& pEffect : m_pSkillEffects)
        Safe_Release(pEffect);

    m_pSkillEffects.clear();
}
