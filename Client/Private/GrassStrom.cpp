#include "GrassStrom.h"

#include "GameInstance.h"
#include "EffectContatiner.h"

CGrassStrom::CGrassStrom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSkillObjectBase(pDevice, pContext)
{
}

CGrassStrom::CGrassStrom(const CGrassStrom& rhs) :
    CSkillObjectBase(rhs)
{
}
 
HRESULT CGrassStrom::Initalize_Prototype()
{
    m_iNumSkill = 5;
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CGrassStrom::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    CEffectContatiner::GAMEOBJECT_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };
    
    m_pGameInstance->Manager_PlaySound(TEXT("Tornado.wav"), CHANNELID::EFFECT2, 1.f);
    auto pGameObject = m_pGameInstance->EffectClone_Object(1, TEXT("Tornado_Effect"), &Desc);
    m_pSkillEffects.push_back(pGameObject);

    return S_OK;
}

void CGrassStrom::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CGrassStrom::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CGrassStrom::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CGrassStrom::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CGrassStrom::ADD_Components()
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

CGrassStrom* CGrassStrom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGrassStrom* pGrassStrom = new CGrassStrom(pDevice, pContext);
    if (FAILED(pGrassStrom->Initalize_Prototype()))
    {
        Safe_Release(pGrassStrom);
        MSG_BOX("CREATE FAILO : Grass Strom");
    }
    return pGrassStrom;
}

CGameObject* CGrassStrom::Clone(void* pArg)
{
    CGrassStrom* pGrassStrom = new CGrassStrom(*this);
    if (FAILED(pGrassStrom->Initialize(pArg)))
    {
        Safe_Release(pGrassStrom);
        MSG_BOX("CLONE FAILO : Grass Strom");
    }
    return pGrassStrom;
}

void CGrassStrom::Free()
{
    __super::Free();
}
