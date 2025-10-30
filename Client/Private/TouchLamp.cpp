#include "TouchLamp.h"

#include "GameInstance.h"
#include "Light.h"

CTouchLamp::CTouchLamp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CArchitecture(pDevice, pContext)
{
}

CTouchLamp::CTouchLamp(const CTouchLamp& rhs) :
    CArchitecture(rhs)
{
}

HRESULT CTouchLamp::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CTouchLamp::Initialize(void* pArg)
{
    m_ObejctTag = TEXT("Torch");
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if( FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pTransformCom->SetScale({ 1.f,1.f,1.f, });
    ArchitectureAction();
    return S_OK;
}

void CTouchLamp::Priority_Update(_float fDeletaTime)
{
    if (m_bIsLight)
    {
        //m_fAccTime -= fDeletaTime;
        if (0 > m_fAccTime)
        {
            //여기서 라이트 제거하고 다시 넣으면 다시 불붙이게
            //m_pGameInstance->RemoveLight(m_pTouchLight);
            m_bIsLight = false;
        }
    }

}

void CTouchLamp::Update(_float fDeletaTime)
{
}

void CTouchLamp::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->ADD_CollisionList(m_pHitBoxCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CTouchLamp::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);
        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render(i);
    }

    m_pHitBoxCollision->Render();
    return S_OK;
}

void CTouchLamp::ArchitectureAction()
{
    m_bIsLight = true;
    m_fLightLifeTime = 3.0f;
}

void CTouchLamp::HitOverlapFunction(_float3 vDir, CGameObject* pHitActor)
{
    //라이트에 오브젝트가 닿으면 damage주기

}

HRESULT CTouchLamp::ADD_Components()
{
    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Torch_Stand"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    CLight::LIGHT_DESC PointLightDesc = {};
    PointLightDesc.eType = LIGHT::POINT;

    PointLightDesc.vDiffuse = _float4(1.0f, 0.55f, 0.25f, 1.0f);
    PointLightDesc.vAmbient = _float4(0.25f, 0.1f, 0.05f, 1.0f);
    PointLightDesc.vSpecular = PointLightDesc.vDiffuse;;
    PointLightDesc.fRange = 1000.f;

    PointLightDesc.vPosition = m_pTransformCom->GetPosition();
    m_pTouchLight = CLight::Create(PointLightDesc);
    if (nullptr == m_pTouchLight)
        return E_FAIL;

    Safe_AddRef(m_pTouchLight);
    m_pGameInstance->ADDLight(m_pTouchLight);


    // 콜리전 정보를 넣어서 한다.
   // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 1.f, 1.f, 1.f };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pHitBoxCollision, &OBBDesc)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CTouchLamp* CTouchLamp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTouchLamp* pTouchLamp = new CTouchLamp(pDevice, pContext);
    if (FAILED(pTouchLamp->Initalize_Prototype()))
    {
        Safe_Release(pTouchLamp);
        MSG_BOX("CREATE FAIL : TOUCH LAMP");
    }
    return pTouchLamp;
}

CGameObject* CTouchLamp::Clone(void* pArg)
{
    CTouchLamp* pTouchLamp = new CTouchLamp(*this);
    if (FAILED(pTouchLamp->Initialize(pArg)))
    {
        Safe_Release(pTouchLamp);
        MSG_BOX("CLONE FAIL : TOUCH LAMP");
    }
    return pTouchLamp;
}

void CTouchLamp::Free()
{
    __super::Free();

    Safe_Release(m_pTouchLight);
}
