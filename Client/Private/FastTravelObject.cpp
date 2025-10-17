#include "FastTravelObject.h"

#include "GameInstance.h"

CFastTravelObject::CFastTravelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CProbObject(pDevice, pContext)
{
}

CFastTravelObject::CFastTravelObject(const CFastTravelObject& rhs) :
    CProbObject(rhs)
{
}

HRESULT CFastTravelObject::Initalize_Prototype()
{

    return S_OK;
}

HRESULT CFastTravelObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CFastTravelObject::Priority_Update(_float fDeletaTime)
{
}

void CFastTravelObject::Update(_float fDeletaTime)
{
    _vector vCalCamereaPos = m_pGameInstance->GetCameraState(WORLDSTATE::POSITION);
    _vector vCalCamereaLook = m_pGameInstance->GetCameraState(WORLDSTATE::LOOK);
    CCollision::DEFAULT_HIT_DESC	RayHitDesc = {};
    if (m_pCollision->RayHit(vCalCamereaPos, vCalCamereaLook, RayHitDesc))
    {
        if (RayHitDesc.vfDistance < m_fActionDistance)
        {
            if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_F))
            {
                //여기서 시간남으면 월드맵 같은거 구현해서 이동할 예정



            }
        }
    }
}

void CFastTravelObject::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->ADD_CollisionList(m_pCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CFastTravelObject::Render()
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

HRESULT CFastTravelObject::ADD_Components()
{
    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_FastTravel"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // 콜리전 정보를 넣어서 한다.
   // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 1.f, 1.f, 1.f };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &OBBDesc)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CFastTravelObject* CFastTravelObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFastTravelObject* pFastTravel = new CFastTravelObject(pDevice, pContext);
    if(FAILED(pFastTravel->Initalize_Prototype()))
    {
        Safe_Release(pFastTravel);
        MSG_BOX("CREATE FAIL : FAST TRAVEL ");
    }
    return pFastTravel;
}

CGameObject* CFastTravelObject::Clone(void* pArg)
{
    CFastTravelObject* pFastTravel = new CFastTravelObject(*this);
    if (FAILED(pFastTravel->Initialize(pArg)))
    {
        Safe_Release(pFastTravel);
        MSG_BOX("CLONE FAIL : FAST TRAVEL ");
    }
    return pFastTravel;
}

void CFastTravelObject::Free()
{
    __super::Free();
}
