#include "Earthquake.h"

#include "GameInstance.h"
#include "EffectContatiner.h"

#include "Player.h"
#include "PellBase.h"

CEarthquake::CEarthquake(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSkillObjectBase(pDevice, pContext)
{
}

CEarthquake::CEarthquake(const CEarthquake& rhs) :
    CSkillObjectBase(rhs)
{
}

HRESULT CEarthquake::Initalize_Prototype()
{
    m_iNumSkill = 4;
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEarthquake::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;
    
    CEffectContatiner::GAMEOBJECT_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };
    auto pGameObject = m_pGameInstance->EffectClone_Object(1, TEXT("Effect_EarthQuake"), &Desc);
    
    m_vSpeed = 0.8f;
    m_fAccTime = 0.f;
    m_fTotalTime = 1.7f;

    m_vStartScale = { 1.f, 5.f, 1.f };
    m_vEndScale = { 7.f, 5.f, 7.f };

    m_pGameInstance->Manager_PlaySound(TEXT("EFF_MON_Short_Burst_Earth_04_A.ogg"), CHANNELID::EFFECT2, 0.5f);
    m_pSkillEffects.push_back(pGameObject);

    return S_OK;
}

void CEarthquake::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);

    m_fAccTime += fDeletaTime * m_vSpeed;
    _vector vStartScale = XMLoadFloat3(&m_vStartScale);
    _vector vEndScale = XMLoadFloat3(&m_vEndScale);

    if (m_fAccTime >= m_fTotalTime)
    {
        if (!m_bIsLerpEnd)
        {
            SetDead(true);
            m_bIsLerpEnd = true;
        }
    }
    else
    {
        _float3 vLerpScale = {};
       _vector vCalLerpScale =  XMVectorLerp(vStartScale, vEndScale, m_fAccTime / m_fTotalTime);

       XMStoreFloat3(&vLerpScale, vCalLerpScale);
       static_cast<COBBCollision*>(m_pCollision)->SetCollision({}, {}, vLerpScale);
    }
}

void CEarthquake::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CEarthquake::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CEarthquake::Render()
{
    __super::Render();
    return S_OK;
}

void CEarthquake::HitOverlapEvent(_float3 vDir, CGameObject* pGameObject)
{
    Default_Damage_Desc DamageDesc = {};
    DamageDesc.fDmaged = m_SkillData.iSkillDamage;

    auto pPlayer = dynamic_cast<CPlayer*>(pGameObject);
    if (pPlayer)
    {
        if (ACTOR_TEAM::FRENDLY == static_cast<CPellBase*>(m_pOwner)->GetPellTeam())
            return;
        else
            __super::HitOverlapEvent(vDir, pGameObject);
    }

    auto pPellBase = dynamic_cast<CPellBase*>(pGameObject);
    if (pPellBase)
    {
        if (pPellBase->GetPellTeam() == static_cast<CPellBase*>(m_pOwner)->GetPellTeam())
            return;
        else
            __super::HitOverlapEvent(vDir, pGameObject);
    }
}

HRESULT CEarthquake::ADD_Components()
{
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    ZeroMemory(&OBBDesc, sizeof(COBBCollision::OBB_COLLISION_DESC));
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = _float3(0.5f, 1.2f, 0.5f);
    OBBDesc.vCneter = _float3(0.f, OBBDesc.vExtents.y * 0.5f, 0.f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &OBBDesc)))
        return E_FAIL;

    m_pCollision->ADD_IgnoreObejct(typeid(CEarthquake).hash_code());
    m_pCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { HitOverlapEvent(vDir, pHitActor); });
    return S_OK;
}

void CEarthquake::Effect_Dead()
{
    for (auto pEffect : m_pSkillEffects)
    {
        if (false == pEffect->IsDead())
            break;
    }

    SetDead(true);
}

CEarthquake* CEarthquake::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEarthquake* pEarthquake = new CEarthquake(pDevice, pContext);
    if (FAILED(pEarthquake->Initalize_Prototype()))
    {
        Safe_Release(pEarthquake);
        MSG_BOX("CREATE FAILO : EarthQuake");
    }
    return pEarthquake;
}

CGameObject* CEarthquake::Clone(void* pArg)
{

    CEarthquake* pEarthquake = new CEarthquake(*this);
    if (FAILED(pEarthquake->Initialize(pArg)))
    {
        Safe_Release(pEarthquake);
        MSG_BOX("CLONE FAILO : EarthQuake");
    }
    return pEarthquake;
}

void CEarthquake::Free()
{
    __super::Free();
}
