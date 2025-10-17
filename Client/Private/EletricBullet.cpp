#include "EletricBullet.h"

#include "GameInstance.h"
#include "Actor.h"

CEletricBullet::CEletricBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CProjectileObject(pDevice, pContext)
{
}

CEletricBullet::CEletricBullet(const CEletricBullet& rhs) :
    CProjectileObject(rhs)
{
}

HRESULT CEletricBullet::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CEletricBullet::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    //if (FAILED(Bind_ShaderResources()))
    //    return E_FAIL;

    return S_OK;
}

void CEletricBullet::Priority_Update(_float fDeletaTime)
{
    m_pTransformCom->ADD_Position(XMLoadFloat3(&m_fThorwDireaction) * m_fThorowSpeed * fDeletaTime);
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));

    m_CombinedWorldMatrix = m_pTransformCom->GetWorldMat();
}

void CEletricBullet::Update(_float fDeletaTime)
{
}

void CEletricBullet::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->ADD_CollisionList(m_pCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
    else
        m_IsDead = true;
}

HRESULT CEletricBullet::Render()
{
    m_pCollision->Render();
    return S_OK;
}

HRESULT CEletricBullet::ADD_Components()
{
    //if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Arrow"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
    //    return E_FAIL;

    //if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
    //    return E_FAIL;

    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 0.1f, 0.3f, 0.1f };
    OBBDesc.vCneter = _float3(0.f, OBBDesc.vExtents.y * 0.5f, 0.f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collison_Com"), (CComponent**)&m_pCollision, &OBBDesc)))
        return E_FAIL;

    m_pCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pTarget) { BeginOverlapEvent(vDir, pTarget); });
    return S_OK;
}

void CEletricBullet::BeginOverlapEvent(_float3 vDir, CGameObject* pTarget)
{
    auto pHitActor = dynamic_cast<CActor*>(pTarget);
    if (pHitActor)
    {
        DEFAULT_DAMAGE_DESC DamageDesc = {};
        DamageDesc.fDmaged = 5.f;

        pHitActor->Damage(&DamageDesc, m_pAttacker);
        m_IsDead = true;
    }
}

CEletricBullet* CEletricBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEletricBullet* pElectricBullet = new CEletricBullet(pDevice, pContext);
    if (FAILED(pElectricBullet->Initalize_Prototype()))
    {
        Safe_Release(pElectricBullet);
        MSG_BOX("CREATE FAIL : Electirc Bullet");
    }
    return pElectricBullet;
}

CGameObject* CEletricBullet::Clone(void* pArg)
{
    CEletricBullet* pElectricBullet = new CEletricBullet(*this);
    if (FAILED(pElectricBullet->Initialize(pArg)))
    {
        Safe_Release(pElectricBullet);
        MSG_BOX("CLONE FAIL : Electirc Bullet");
    }
    return pElectricBullet;
}

void CEletricBullet::Free()
{
    __super::Free();
}
