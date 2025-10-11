#include "WoodWall.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CWoodWall::CWoodWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CArchitecture(pDevice, pContext)
{
}

CWoodWall::CWoodWall(const CWoodWall& rhs) :
    CArchitecture(rhs)
{
}

HRESULT CWoodWall::Initalize_Prototype()
{
    m_ItemID = ARCHITECTURE_WOOD_WALL;
    return S_OK;
}

HRESULT CWoodWall::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pHitBoxCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    m_fActionDistance = 100.f;
    return S_OK;
}

void CWoodWall::Priority_Update(_float fDeletaTime)
{
}

void CWoodWall::Update(_float fDeletaTime)
{
}

void CWoodWall::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->ADD_CollisionList(m_pHitBoxCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CWoodWall::Render()
{
    _uInt iNumMesh = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMesh; ++i)
    {
        Apply_ConstantShaderResources(i);
        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render(i);
    }
    m_pHitBoxCollision->Render();
    return S_OK;
}

void CWoodWall::Damage(void* pArg, CActor* pAttackActor)
{
    DEFAULT_DAMAGE_DESC* DamageDesc = static_cast<DEFAULT_DAMAGE_DESC*>(pArg);
    // 체력이 0 되면 부서짐
}

HRESULT CWoodWall::ADD_Components()
{
    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_WoodWall"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // 콜리전 정보를 넣어서 한다.
   // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 4.f, 4.f, 1.f };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pHitBoxCollision, &OBBDesc)))
        return E_FAIL;
    m_pHitBoxCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { HitOverlapFunction(vDir, pHitActor); });

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CWoodWall* CWoodWall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWoodWall* pWoodWall = new CWoodWall(pDevice, pContext);
    if (FAILED(pWoodWall->Initalize_Prototype()))
    {
        Safe_Release(pWoodWall);
        MSG_BOX("CREATE FAIL : WOOD WALL");
    }
    return pWoodWall;
}

CGameObject* CWoodWall::Clone(void* pArg)
{
    CWoodWall* pWoodWall = new CWoodWall(*this);
    if (FAILED(pWoodWall->Initialize(pArg)))
    {
        Safe_Release(pWoodWall);
        MSG_BOX("CLONE FAIL : WOOD WALL");
    }
    return pWoodWall;
}

void CWoodWall::Free()
{
    __super::Free();
}
