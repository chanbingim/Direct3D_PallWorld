#include "TreeObject.h"

#include "GameInstance.h"
#include "TerrainManager.h"

#include "DropComponent.h"

CTreeObject::CTreeObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CEnviormnent(pDevice, pContext)
{
}

CTreeObject::CTreeObject(const CTreeObject& rhs) : 
    CEnviormnent(rhs)
{
}

HRESULT CTreeObject::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    m_eWorkType = PELL_WORK_TYPE::TREE_GATHER;
    return S_OK;
}

HRESULT CTreeObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    ENVIORMNENT_DESC* pEnvDesc = static_cast<ENVIORMNENT_DESC*>(pArg);
    if (FAILED(ADD_Components(pEnvDesc->iModelIndex)))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    RefreshComputeHeight();
    m_pDropComponent->Insert_ItemIndex(11, 100);
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));

    return S_OK;
}

void CTreeObject::Priority_Update(_float fDeletaTime)
{
}

void CTreeObject::Update(_float fDeletaTime)
{
}

void CTreeObject::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->ADD_CollisionList(m_pCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CTreeObject::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render(i);
    }
    m_pCollision->Render();

    return S_OK;
}

HRESULT CTreeObject::DeadFunction()
{
    if (FAILED(__super::DeadFunction()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTreeObject::HitBeginFunction(_float3 vDir, CGameObject* pGameObject)
{
    if (FAILED(__super::HitBeginFunction(vDir, pGameObject)))
        return E_FAIL;

    return S_OK;
}

void CTreeObject::Damage(void* pArg, CActor* pDamagedActor)
{
    __super::Damage(pArg, pDamagedActor);
}

HRESULT CTreeObject::ADD_Components(_uInt iModelIndex)
{
    WCHAR szModelName[MAX_PATH] = {};
    wsprintf(szModelName, TEXT("Prototype_Component_VIBuffer_Tree%d_Mesh"), iModelIndex);

    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), szModelName, TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;
    
    // 콜리전 정보를 넣어서 한다.
   // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 1.f, 1.f, 1.f };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &OBBDesc)))
        return E_FAIL;

    m_pCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { HitBeginFunction(vDir, pHitActor); });
    m_pCollision->BindOverlappingEvent([this](_float3 vDir, CGameObject* pHitActor) { HitOverlapFunction(vDir, pHitActor); });

    // DropComponent
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_Component_Drop"), TEXT("Drop_Com"), (CComponent**)&m_pDropComponent)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CTreeObject* CTreeObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTreeObject* pTreeObject = new CTreeObject(pDevice, pContext);
    if (FAILED(pTreeObject->Initalize_Prototype()))
    {
        Safe_Release(pTreeObject);
        MSG_BOX("CREATE FAIL : TREE OBJECT");
    }
    return pTreeObject;
}

CGameObject* CTreeObject::Clone(void* pArg)
{
    CTreeObject* pTreeObject = new CTreeObject(*this);
    if (FAILED(pTreeObject->Initialize(pArg)))
    {
        Safe_Release(pTreeObject);
        MSG_BOX("CLONE FAIL : TREE OBJECT");
    }
    return pTreeObject;
}

void CTreeObject::Free()
{
    __super::Free();
}
