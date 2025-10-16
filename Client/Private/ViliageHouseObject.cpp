#include "ViliageHouseObject.h"

#include "GameInstance.h"

CViliageHouseObject::CViliageHouseObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CProbObject(pDevice, pContext)
{
}

CViliageHouseObject::CViliageHouseObject(const CViliageHouseObject& rhs) :
    CProbObject(rhs)
{
}

HRESULT CViliageHouseObject::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CViliageHouseObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CViliageHouseObject::Priority_Update(_float fDeletaTime)
{
}

void CViliageHouseObject::Update(_float fDeletaTime)
{
}

void CViliageHouseObject::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->ADD_CollisionList(m_pCollision);
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CViliageHouseObject::Render()
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

HRESULT CViliageHouseObject::ADD_Components()
{
    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_PalBox"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
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

CViliageHouseObject* CViliageHouseObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CViliageHouseObject* pViliageHouse = new CViliageHouseObject(pDevice, pContext);
    if (FAILED(pViliageHouse->Initalize_Prototype()))
    {
        Safe_Release(pViliageHouse);
        MSG_BOX("CREATE FAIL : Viliage House ");
    }
    return pViliageHouse;
}

CGameObject* CViliageHouseObject::Clone(void* pArg)
{
    CViliageHouseObject* pViliageHouse = new CViliageHouseObject(*this);
    if (FAILED(pViliageHouse->Initialize(pArg)))
    {
        Safe_Release(pViliageHouse);
        MSG_BOX("CREATE FAIL : Viliage House ");
    }
    return pViliageHouse;
}

void CViliageHouseObject::Free()
{
    __super::Free();
}