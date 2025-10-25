#include "Earthquake.h"

#include "GameInstance.h"
#include "EffectContatiner.h"

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
    auto pGameObject = m_pGameInstance->EffectClone_Object(1, TEXT("EarthQuake_Effect"), &Desc);
    m_pSkillEffects.push_back(pGameObject);

    return S_OK;
}

void CEarthquake::Priority_Update(_float fDeletaTime)
{
    // 이거 Lerp라서 내가 받아서 해보자
    
    __super::Priority_Update(fDeletaTime);
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

HRESULT CEarthquake::ADD_Components()
{
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    ZeroMemory(&OBBDesc, sizeof(COBBCollision::OBB_COLLISION_DESC));
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = _float3(0.5f, 1.2f, 0.5f);
    OBBDesc.vCneter = _float3(0.f, OBBDesc.vExtents.y * 0.5f, 0.f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &OBBDesc)))
        return E_FAIL;

    return S_OK;
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
