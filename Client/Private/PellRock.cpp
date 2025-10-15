#include "PellRock.h"

#include "GameInstance.h"
#include "TerrainManager.h"

#include "DropComponent.h"

CPellRock::CPellRock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMiningObject(pDevice, pContext)
{
}

CPellRock::CPellRock(const CPellRock& rhs) :
    CMiningObject(rhs)
{
}

HRESULT CPellRock::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    m_eWorkType = PELL_WORK_TYPE::MINING;
    return S_OK;
}

HRESULT CPellRock::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    auto pNaviMesh = CTerrainManager::GetInstance()->GetNavimesh();
    pNaviMesh->ComputeHeight(m_pTransformCom, true);

    m_pDropComponent->Insert_ItemIndex(13, 100);
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));

    return S_OK;
}

void CPellRock::Priority_Update(_float fDeletaTime)
{
}

void CPellRock::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CPellRock::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->ADD_CollisionList(m_pCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CPellRock::Render()
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

HRESULT CPellRock::DeadFunction()
{
    if (FAILED(__super::DeadFunction()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPellRock::HitBeginFunction(_float3 vDir, CGameObject* pGameObject)
{
    if(FAILED(__super::HitBeginFunction(vDir, pGameObject)))
        return E_FAIL;

    return S_OK;
}

void CPellRock::Damage(void* pArg, CActor* pDamagedActor)
{
    __super::Damage(pArg, pDamagedActor);
}

HRESULT CPellRock::ADD_Components()
{
      // 펠 광석 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_PellJium_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // 콜리전 정보를 넣어서 한다.
// 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 0.5f, 0.5f, 0.5f };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &OBBDesc)))
        return E_FAIL;
    m_pCollision->BindOverlappingEvent([this](_float3 vDir, CGameObject* pHitActor) { HitOverlapFunction(vDir, pHitActor); });

    // DropComponent
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_Component_Drop"), TEXT("Drop_Com"), (CComponent**)&m_pDropComponent)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CPellRock* CPellRock::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPellRock* pPellRock = new CPellRock(pDevice, pContext);
    if (FAILED(pPellRock->Initalize_Prototype()))
    {
        Safe_Release(pPellRock);
        MSG_BOX("CREATE FAIL : PELL ROCK OBJECT");
    }
    return pPellRock;
}

CGameObject* CPellRock::Clone(void* pArg)
{
    CPellRock* pPellRock = new CPellRock(*this);
    if (FAILED(pPellRock->Initialize(pArg)))
    {
        Safe_Release(pPellRock);
        MSG_BOX("CLONE FAIL : PELL ROCK OBJECT");
    }
    return pPellRock;
}

void CPellRock::Free()
{
    __super::Free();
}
