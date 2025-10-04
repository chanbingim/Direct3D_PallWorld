#include "PalBox.h"

#include "GameInstance.h"

CPalBox::CPalBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CArchitecture(pDevice, pContext)
{
}

CPalBox::CPalBox(const CPalBox& rhs) :
    CArchitecture(rhs)
{
}

HRESULT CPalBox::Initalize_Prototype()
{
    m_ItemID = ARCHITECTURE_PALBOX;
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalBox::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pDebugSphere->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    m_pHitBoxCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    return S_OK;
}

void CPalBox::Priority_Update(_float fDeletaTime)
{
}

void CPalBox::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CPalBox::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
        m_pGameInstance->ADD_CollisionList(m_pHitBoxCollision);
    }
}

HRESULT CPalBox::Render()
{
    _uInt iNumMesh = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0;  i < iNumMesh; ++i)
    {
        Apply_ConstantShaderResources(i);

        if(m_bIsCompleted)
            m_pShaderCom->Update_Shader(0);
        else
            m_pShaderCom->Update_Shader(2);

        m_pVIBufferCom->Render(i);
    }

    m_pHitBoxCollision->Render();
    m_pDebugSphere->Render();

    return S_OK;
}

void CPalBox::ArchitectureAction()
{

}

void CPalBox::HitOverlapFunction(_float3 vDir, CGameObject* pHitActor)
{
    __super::HitOverlapFunction(vDir, pHitActor);
}

HRESULT CPalBox::ADD_Components()
{
    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_PalBox"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // 콜리전 정보를 넣어서 한다.
   // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 1.f, 1.f, 1.f };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pHitBoxCollision, &OBBDesc)))
        return E_FAIL;
    m_pHitBoxCollision->BindOverlappingEvent([this](_float3 vDir, CGameObject* pHitActor) { HitOverlapFunction(vDir, pHitActor); });

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

#ifdef _DEBUG
    // 콜리전 정보를 넣어서 한다.
  // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    CSphereCollision::SPEHRE_COLLISION_DESC SphereDesc = {};
    SphereDesc.pOwner = this;
    SphereDesc.Radius = 30.f; // m_fAreaRadius;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionSphere"), TEXT("AreaCollision_Com"), (CComponent**)&m_pDebugSphere, &SphereDesc)))
        return E_FAIL;
#endif // _DEBUG

    return S_OK;
}

CPalBox* CPalBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPalBox* pPalBox = new CPalBox(pDevice, pContext);
    if (FAILED(pPalBox->Initalize_Prototype()))
    {
        Safe_Release(pPalBox);
        MSG_BOX("CREATE FAIL : PAL BOX");
    }
    return pPalBox;
}

CGameObject* CPalBox::Clone(void* pArg)
{
    CPalBox* pPalBox = new CPalBox(*this);
    if (FAILED(pPalBox->Initialize(pArg)))
    {
        Safe_Release(pPalBox);
        MSG_BOX("CLONE FAIL : PAL BOX");
    }
    return pPalBox;
}

void CPalBox::Free()
{
    __super::Free();

#ifdef _DEBUG
    Safe_Release(m_pDebugSphere);
#endif // _DEBUG
}
