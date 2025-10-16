#include "TableFurniture.h"

#include "GameInstance.h"

CTableFurniture::CTableFurniture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CArchitecture(pDevice, pContext)
{
}

CTableFurniture::CTableFurniture(const CTableFurniture& rhs) :
    CArchitecture(rhs)
{
}

HRESULT CTableFurniture::Initalize_Prototype()
{
    m_ItemID = ARCHITECTURE_TABLE;
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTableFurniture::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pHitBoxCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    return S_OK;
}

void CTableFurniture::Priority_Update(_float fDeletaTime)
{
}

void CTableFurniture::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CTableFurniture::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
        m_pGameInstance->ADD_CollisionList(m_pHitBoxCollision);
    }
}

HRESULT CTableFurniture::Render()
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

void CTableFurniture::HitOverlapFunction(_float3 vDir, CGameObject* pHitActor)
{
    __super::HitOverlapFunction(vDir, pHitActor);
}

HRESULT CTableFurniture::ADD_Components()
{
    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_PalBed"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // 콜리전 정보를 넣어서 한다.
   // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 1.f, 1.f, 1.f };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pHitBoxCollision, &OBBDesc)))
        return E_FAIL;
    m_pHitBoxCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { HitOverlapFunction(vDir, pHitActor); });

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CTableFurniture* CTableFurniture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTableFurniture* pTableTurniture = new CTableFurniture(pDevice, pContext);
    if (FAILED(pTableTurniture->Initalize_Prototype()))
    {
        Safe_Release(pTableTurniture);
        MSG_BOX("CREATE FAIL : TABLE FURNITURE");
    }
    return pTableTurniture;
}

CGameObject* CTableFurniture::Clone(void* pArg)
{
    CTableFurniture* pTableTurniture = new CTableFurniture(*this);
    if (FAILED(pTableTurniture->Initialize(pArg)))
    {
        Safe_Release(pTableTurniture);
        MSG_BOX("CLONE FAIL : TABLE FURNITURE");
    }
    return pTableTurniture;
}

void CTableFurniture::Free()
{
    __super::Free();
}
