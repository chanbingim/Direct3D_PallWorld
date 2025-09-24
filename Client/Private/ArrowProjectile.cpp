#include "ArrowProjectile.h"

#include "GameInstance.h"
#include "Actor.h"

CArrowProjectile::CArrowProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CProjectileObject(pDevice, pContext)
{
}

CArrowProjectile::CArrowProjectile(const CArrowProjectile& rhs) :
    CProjectileObject(rhs)
{
}

HRESULT CArrowProjectile::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CArrowProjectile::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CArrowProjectile::Priority_Update(_float fDeletaTime)
{
    m_pTransformCom->ADD_Position(XMLoadFloat3(&m_fThorwDireaction) * m_fThorowSpeed * fDeletaTime);
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));

    m_CombinedWorldMatrix = m_pTransformCom->GetWorldMat();
}

void CArrowProjectile::Update(_float fDeletaTime)
{

}

void CArrowProjectile::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->ADD_CollisionList(m_pCollision);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CArrowProjectile::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(0);

        m_pVIBufferCom->Render(i);
    }
    return S_OK;
}

HRESULT CArrowProjectile::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Arrow"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 0.1f, 0.3f, 0.1f};
    OBBDesc.vCneter = _float3(0.f, OBBDesc.vExtents.y * 0.5f, 0.f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collison_Com"), (CComponent**)&m_pCollision, &OBBDesc)))
        return E_FAIL;

    m_pCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pTarget) { BeginOverlapEvent(vDir, pTarget); });
    return S_OK;
}

void CArrowProjectile::BeginOverlapEvent(_float3 vDir, CGameObject* pTarget)
{
    auto pHitActor = dynamic_cast<CActor*>(pTarget);
    if (pHitActor)
    {
        DEFAULT_DAMAGE_DESC DamageDesc = {};
        DamageDesc.fDmaged = 5.f;

        pHitActor->Damage(&DamageDesc, m_pAttacker);
    }
}

CArrowProjectile* CArrowProjectile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CArrowProjectile* pArrowProjectTile = new CArrowProjectile(pDevice, pContext);
    if (FAILED(pArrowProjectTile->Initalize_Prototype()))
    {
        Safe_Release(pArrowProjectTile);
        MSG_BOX("CREATE FAIL : ARROW PORJECTILE"); 
    }
    return pArrowProjectTile;
}

CGameObject* CArrowProjectile::Clone(void* pArg)
{
    CArrowProjectile* pArrowProjectTile = new CArrowProjectile(*this);
    if (FAILED(pArrowProjectTile->Initialize(pArg)))
    {
        Safe_Release(pArrowProjectTile);
        MSG_BOX("CLONE FAIL : ARROW PORJECTILE");
    }
    return pArrowProjectTile;
}

void CArrowProjectile::Free()
{
    __super::Free();
}
