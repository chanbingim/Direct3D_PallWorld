#include "RockObject.h"

#include "GameInstance.h"


#include "DropComponent.h"

CRockObject::CRockObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMiningObject(pDevice, pContext)
{
}

CRockObject::CRockObject(const CRockObject& rhs) :
    CMiningObject(rhs)
{
}

HRESULT CRockObject::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    m_eWorkType = PELL_WORK_TYPE::MINING;
    return S_OK;
}

HRESULT CRockObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    ENVIORMNENT_DESC* pEnvDesc = static_cast<ENVIORMNENT_DESC*>(pArg);
    if (FAILED(ADD_Components(pEnvDesc->iModelIndex)))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    RefreshComputeHeight();
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    m_pDropComponent->Insert_ItemIndex(12, 100);
    return S_OK;
}

void CRockObject::Priority_Update(_float fDeletaTime)
{
}

void CRockObject::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CRockObject::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->ADD_CollisionList(m_pCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CRockObject::Render()
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

HRESULT CRockObject::DeadFunction()
{
    if (FAILED(__super::DeadFunction()))
        return E_FAIL;

    return S_OK;
}

HRESULT CRockObject::HitBeginFunction(_float3 vDir, CGameObject* pGameObject)
{
    if (FAILED(__super::HitBeginFunction(vDir, pGameObject)))
        return E_FAIL;

    return S_OK;
}

void CRockObject::Damage(void* pArg, CActor* pDamagedActor)
{
    __super::Damage(pArg, pDamagedActor);
}

HRESULT CRockObject::ADD_Components(_uInt iModelIndex)
{
    WCHAR szModelName[MAX_PATH] = {};
    wsprintf(szModelName, TEXT("Prototype_Component_VIBuffer_Rock_%d_Mesh"), iModelIndex);

    // 돌  모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), szModelName, TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // 콜리전 정보를 넣어서 한다.
    // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자s
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    if (2 == iModelIndex)
    {
        OBBDesc.vExtents = { 1.5f, 1.f, 1.0f };
        m_fCompleteTime = 3.f;
    }
    else if(1 == iModelIndex)
    {
        OBBDesc.vExtents = { 1.f, 1.f, 1.f };
        m_fCompleteTime = 3.f;
    }
    else if (0 == iModelIndex)
    {
        OBBDesc.vExtents = { 1.5f, 2.3f, 2.5f };
        m_fCompleteTime = 5.f;
    }

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

CRockObject* CRockObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CRockObject* pRockObject = new CRockObject(pDevice, pContext);
    if (FAILED(pRockObject->Initalize_Prototype()))
    {
        Safe_Release(pRockObject);
        MSG_BOX("CREATE FAIL : ROCK OBJECT");
    }
    return pRockObject;
}

CGameObject* CRockObject::Clone(void* pArg)
{
    CRockObject* pRockObject = new CRockObject(*this);
    if (FAILED(pRockObject->Initialize(pArg)))
    {
        Safe_Release(pRockObject);
        MSG_BOX("CLONE FAIL : ROCK OBJECT");
    }
    return pRockObject;
}

void CRockObject::Free()
{
    __super::Free();
}