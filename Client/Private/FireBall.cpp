#include "FireBall.h"

#include "GameInstance.h"
#include "EffectContatiner.h"

CFireBall::CFireBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSkillObjectBase(pDevice, pContext)
{
}

CFireBall::CFireBall(const CFireBall& rhs) :
    CSkillObjectBase(rhs)
{
}

HRESULT CFireBall::Initalize_Prototype()
{
    m_iNumSkill = 8;
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CFireBall::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    CEffectContatiner::GAMEOBJECT_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };
    auto pGameObject = m_pGameInstance->EffectClone_Object(1, TEXT("fIRE_Effect"), &Desc);
    m_pSkillEffects.push_back(pGameObject);

    m_fChargeTime = 1.f;
    m_bIsCharge = true;
    return S_OK;
}

void CFireBall::Priority_Update(_float fDeletaTime)
{
    if(!m_bIsCharge)
        __super::Priority_Update(fDeletaTime);
    else
    {
        m_fChargeTime -= fDeletaTime;

        if (0 >= m_fChargeTime)
            m_bIsCharge = false;
    }
}

void CFireBall::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CFireBall::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CFireBall::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CFireBall::ADD_Components()
{
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    ZeroMemory(&OBBDesc, sizeof(COBBCollision::OBB_COLLISION_DESC));
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = _float3(1.f, 1.2f, 1.f);
    OBBDesc.vCneter = _float3(0.f, OBBDesc.vExtents.y * 0.5f, 0.f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &OBBDesc)))
        return E_FAIL;

    m_pCollision->BindBeginOverlapEvent([&](_float3 vDir, CGameObject* pGameObject) { HitOverlapEvent(vDir, pGameObject); });
    m_pCollision->BindOverlappingEvent([&](_float3 vDir, CGameObject* pGameObject) { HitOverlapping(vDir, pGameObject); });

    return S_OK;
}

CFireBall* CFireBall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFireBall* pFireBall = new CFireBall(pDevice, pContext);
    if (FAILED(pFireBall->Initalize_Prototype()))
    {
        Safe_Release(pFireBall);
        MSG_BOX("CREATE FAILO : Fire Ball");
    }
    return pFireBall;
}

CGameObject* CFireBall::Clone(void* pArg)
{
    CFireBall* pFireBall = new CFireBall(*this);
    if (FAILED(pFireBall->Initialize(pArg)))
    {
        Safe_Release(pFireBall);
        MSG_BOX("CLONE FAILO : Fire Ball");
    }
    return pFireBall;
}

void CFireBall::Free()
{
    __super::Free();
}
