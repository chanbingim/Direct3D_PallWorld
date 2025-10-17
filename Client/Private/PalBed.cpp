#include "PalBed.h"

#include "GameInstance.h"

CPalBed::CPalBed(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CArchitecture(pDevice, pContext)
{
}

CPalBed::CPalBed(const CPalBed& rhs) : 
    CArchitecture(rhs)
{
}

HRESULT CPalBed::Initalize_Prototype()
{
    m_ItemID = ARCHITECTURE_PALBED;
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalBed::Initialize(void* pArg)
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

void CPalBed::Priority_Update(_float fDeletaTime)
{
}

void CPalBed::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CPalBed::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
        m_pGameInstance->ADD_CollisionList(m_pHitBoxCollision);
    }
}

HRESULT CPalBed::Render()
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

void CPalBed::HitOverlapFunction(_float3 vDir, CGameObject* pHitActor)
{
    __super::HitOverlapFunction(vDir, pHitActor);
}

HRESULT CPalBed::ADD_Components()
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

CPalBed* CPalBed::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPalBed* pPalBed = new CPalBed(pDevice, pContext);
    if (FAILED(pPalBed->Initalize_Prototype()))
    {
        Safe_Release(pPalBed);
        MSG_BOX("CREATE FAIL : PAL BED");
    }
    return pPalBed;
}

CGameObject* CPalBed::Clone(void* pArg)
{
    CPalBed* pPalBed = new CPalBed(*this);
    if (FAILED(pPalBed->Initialize(pArg)))
    {
        Safe_Release(pPalBed);
        MSG_BOX("CLONE FAIL : PAL BED");
    }
    return pPalBed;
}

void CPalBed::Free()
{
    __super::Free();
}
