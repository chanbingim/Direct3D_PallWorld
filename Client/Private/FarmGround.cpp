#include "FarmGround.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CFarmGround::CFarmGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CArchitecture(pDevice, pContext)
{
}

CFarmGround::CFarmGround(const CFarmGround& rhs) :
	CArchitecture(rhs)
{
}

HRESULT CFarmGround::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CFarmGround::Initialize(void* pArg)
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

void CFarmGround::Priority_Update(_float fDeletaTime)
{
}

void CFarmGround::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

}

void CFarmGround::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->ADD_CollisionList(m_pHitBoxCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CFarmGround::Render()
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

void CFarmGround::Damage(void* pArg, CActor* pAttackActor)
{
    DEFAULT_DAMAGE_DESC* DamageDesc = static_cast<DEFAULT_DAMAGE_DESC*>(pArg);
    // 체력이 0 되면 부서짐

}

HRESULT CFarmGround::ADD_Components()
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
    m_pHitBoxCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { HitOverlapFunction(vDir, pHitActor); });

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CFarmGround* CFarmGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFarmGround* pFarmGround = new CFarmGround(pDevice, pContext);
    if (FAILED(pFarmGround->Initalize_Prototype()))
    {
        Safe_Release(pFarmGround);
        MSG_BOX("CREATE FAIL : Farm Ground ");
    }
    return pFarmGround;
}

CGameObject* CFarmGround::Clone(void* pArg)
{
    CFarmGround* pFarmGround = new CFarmGround(*this);
    if (FAILED(pFarmGround->Initialize(pArg)))
    {
        Safe_Release(pFarmGround);
        MSG_BOX("CLONE FAIL : Farm Ground ");
    }
    return pFarmGround;
}

void CFarmGround::Free()
{
    __super::Free();
}
