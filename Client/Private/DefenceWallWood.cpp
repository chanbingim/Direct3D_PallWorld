#include "DefenceWallWood.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CDefenceWallWood::CDefenceWallWood(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CArchitecture(pDevice, pContext)
{
}

CDefenceWallWood::CDefenceWallWood(const CDefenceWallWood& rhs) :
    CArchitecture(rhs)
{ 
}

HRESULT CDefenceWallWood::Initalize_Prototype()
{
    m_ItemID = ARCHITECTURE_DEFENCE_WOOD_WALL;

    return S_OK;
}

HRESULT CDefenceWallWood::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pHitBoxCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    m_fActionDistance = 100.f;
    return S_OK;
}

void CDefenceWallWood::Priority_Update(_float fDeletaTime)
{

}

void CDefenceWallWood::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    _float3 vArchitecturePos = m_pTransformCom->GetPosition();
    _vector vPlayerPos = m_pGameInstance->GetPlayerState(WORLDSTATE::POSITION);
    _float fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vArchitecturePos) - vPlayerPos));
    if (m_fActionDistance <= fLength)
    {
        _vector vCalCamereaPos = m_pGameInstance->GetCameraState(WORLDSTATE::POSITION);
        _vector vCalCamereaLook = m_pGameInstance->GetCameraState(WORLDSTATE::LOOK);

        _vector vCameraToArchDir = XMVector3Normalize(XMLoadFloat3(&vArchitecturePos) - vCalCamereaPos);
        _float fRad = acosf(XMVectorGetX(XMVector3Dot(vCameraToArchDir, vCalCamereaLook)));
        if (fRad <= XM_PIDIV4)
        {
            // 여기서 성벽의 HP를 보여준다.
            //m_pActionUI->Update(fDeletaTime);
        }
    }
}

void CDefenceWallWood::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->ADD_CollisionList(m_pHitBoxCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CDefenceWallWood::Render()
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

void CDefenceWallWood::Damage(void* pArg, CActor* pAttackActor)
{
    DEFAULT_DAMAGE_DESC* DamageDesc = static_cast<DEFAULT_DAMAGE_DESC*>(pArg);
    // 체력이 0 되면 부서짐




}

HRESULT CDefenceWallWood::ADD_Components()
{
    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_DefenceWoodWall"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // 콜리전 정보를 넣어서 한다.
   // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 6.f, 3.f, 1.f };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pHitBoxCollision, &OBBDesc)))
        return E_FAIL;
    m_pHitBoxCollision->BindOverlappingEvent([this](_float3 vDir, CGameObject* pHitActor) { HitOverlapFunction(vDir, pHitActor); });

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CDefenceWallWood* CDefenceWallWood::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDefenceWallWood* pDefenceWoodWall = new CDefenceWallWood(pDevice, pContext);
    if (FAILED(pDefenceWoodWall->Initalize_Prototype()))
    {
        Safe_Release(pDefenceWoodWall);
        MSG_BOX("CREATE FAIL : DEFENCE WOOD WALL");
    }
    return pDefenceWoodWall;
}

CGameObject* CDefenceWallWood::Clone(void* pArg)
{
    CDefenceWallWood* pDefenceWoodWall = new CDefenceWallWood(*this);
    if (FAILED(pDefenceWoodWall->Initialize(pArg)))
    {
        Safe_Release(pDefenceWoodWall);
        MSG_BOX("CLONE FAIL : DEFENCE WOOD WALL");
    }
    return pDefenceWoodWall;
}

void CDefenceWallWood::Free()
{
    __super::Free();
}
