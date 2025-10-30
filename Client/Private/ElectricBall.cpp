#include "ElectricBall.h"

#include "GameInstance.h"
#include "EffectContatiner.h"

CElectricBall::CElectricBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSkillObjectBase(pDevice, pContext)
{
}

CElectricBall::CElectricBall(const CElectricBall& rhs) :
    CSkillObjectBase(rhs)
{
}

HRESULT CElectricBall::Initalize_Prototype()
{
    m_iNumSkill = 7;
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;
    return S_OK;
}

HRESULT CElectricBall::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    m_fChargeTime = 1.5f;
    CEffectContatiner::GAMEOBJECT_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };
    m_pGameInstance->Manager_PlaySound(TEXT("ElectircBall.ogg"), CHANNELID::EFFECT, 1.f);
    auto pGameObject = m_pGameInstance->EffectClone_Object(1, TEXT("Electirc_Ball_Effect"), &Desc);
    m_pSkillEffects.push_back(pGameObject);

    return S_OK;
}

void CElectricBall::Priority_Update(_float fDeletaTime)
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

void CElectricBall::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CElectricBall::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CElectricBall::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CElectricBall::ADD_Components()
{
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    ZeroMemory(&OBBDesc, sizeof(COBBCollision::OBB_COLLISION_DESC));
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = _float3(1.f, 1.2f, 1.f);
    OBBDesc.vCneter = _float3(0.f, OBBDesc.vExtents.y * 0.5f, 0.f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &OBBDesc)))
        return E_FAIL;

    m_pCollision->BindBeginOverlapEvent([&](_float3 vDir, CGameObject* pGameObject) { HitOverlapEvent(vDir, pGameObject); });

    return S_OK;
}

CElectricBall* CElectricBall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CElectricBall* pEletricBall = new CElectricBall(pDevice, pContext);
    if (FAILED(pEletricBall->Initalize_Prototype()))
    {
        Safe_Release(pEletricBall);
        MSG_BOX("CREATE FAILO : Electric Ball");
    }
    return pEletricBall;
}

CGameObject* CElectricBall::Clone(void* pArg)
{
    CElectricBall* pEletricBall = new CElectricBall(*this);
    if (FAILED(pEletricBall->Initialize(pArg)))
    {
        Safe_Release(pEletricBall);
        MSG_BOX("CLONE FAILO : Electric Ball");
    }
    return pEletricBall;
}

void CElectricBall::Free()
{
    __super::Free();
}
