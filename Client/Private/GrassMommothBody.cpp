#include "GrassMommothBody.h"

#include "GameInstance.h"

#include "PellBase.h"
#include "CombatComponent.h"

#include "GrassStrom.h"
#include "Earthquake.h"

CGrassMommothBody::CGrassMommothBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPellBody(pDevice, pContext)
{
}

CGrassMommothBody::CGrassMommothBody(const CGrassMommothBody& rhs) :
    CPellBody(rhs)
{
}

HRESULT CGrassMommothBody::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CGrassMommothBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(Insert_AnimKeyFrameFunction()))
        return E_FAIL;

    m_ProjectileSocketDistance = 5.f;
    return S_OK;
}

void CGrassMommothBody::Priority_Update(_float fDeletaTime)
{
}

void CGrassMommothBody::Update(_float fDeletaTime)
{
    m_bIsAnimFinished = m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime, 10.f, m_bIsAnimLoop);
}

void CGrassMommothBody::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CGrassMommothBody::Render()
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

HRESULT CGrassMommothBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_GrassMommoth_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CGrassMommothBody::Insert_AnimKeyFrameFunction()
{
    m_pVIBufferCom->Bind_KeyFrameFunction("Earthquake", 56, [this]() { EarthquakeEvent(); });
    m_pVIBufferCom->Bind_KeyFrameFunction("FarSkill_ActionLoop", 3, [this]() { FarSkillEvent(); });

    return S_OK;
}

void CGrassMommothBody::EarthquakeEvent()
{
    CEarthquake::SKILL_OBJECT_DESC EarthQuakeDesc = {};
    EarthQuakeDesc.vScale = { 1.f, 1.f, 1.f };
    EarthQuakeDesc.vPosition = m_pTransformCom->GetPosition();
    
    auto pEarthQuake = static_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Pal_Skill_Earthquake"), &EarthQuakeDesc));
    m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("GamePlay_Layer_Skill"), pEarthQuake);
}

void CGrassMommothBody::FarSkillEvent()
{
    _float3 vScale = m_pTransformCom->GetScale();
    _float3 vPosition = m_pParent->GetTransform()->GetPosition();

    _vector vLook = m_pTransformCom->GetLookVector();
    _vector vRight = m_pTransformCom->GetRightVector();
    vLook *= vScale.z * 0.5f;

    _vector vCalPostion = XMLoadFloat3(&vPosition);
    vRight *= m_ProjectileSocketDistance;

    CGrassStrom::SKILL_OBJECT_DESC GrassStromDesc = {};
    CPellBase* pOwner = static_cast<CPellBase*>(m_pParent);
    CCombatComponent* pCombatCom = static_cast<CCombatComponent *>(pOwner->Find_Component(TEXT("Combat_Com")));

    _float3 vTargetPoint = pCombatCom->GetCurrentTarget()->GetTransform()->GetPosition();
     XMStoreFloat3(&GrassStromDesc.vTargetDir, XMVector3Normalize(XMLoadFloat3(&vTargetPoint) - vCalPostion));

    GrassStromDesc.vScale = { 2.f, 2.f, 2.f };
    XMStoreFloat3(&GrassStromDesc.vPosition, vCalPostion + vLook + vRight);

    CGameObject* pGrassStrom = static_cast<CGameObject *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Pal_Skill_GrassStrom"), &GrassStromDesc));
    m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("GamePlay_Layer_Skill"), pGrassStrom);

    XMStoreFloat3(&GrassStromDesc.vPosition, vCalPostion + vLook - vRight);

    pGrassStrom = static_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Pal_Skill_GrassStrom"), &GrassStromDesc));
    m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("GamePlay_Layer_Skill"), pGrassStrom);
}

CGrassMommothBody* CGrassMommothBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGrassMommothBody* pGrassMommothBody = new CGrassMommothBody(pDevice, pContext);
    if (FAILED(pGrassMommothBody->Initalize_Prototype()))
    {
        Safe_Release(pGrassMommothBody);
        MSG_BOX("CREATE FAIL : Grass Mommoth BODY");
    }
    return pGrassMommothBody;
}

CGameObject* CGrassMommothBody::Clone(void* pArg)
{
    CGrassMommothBody* pGrassMommothBody = new CGrassMommothBody(*this);
    if (FAILED(pGrassMommothBody->Initialize(pArg)))
    {
        Safe_Release(pGrassMommothBody);
        MSG_BOX("CLONE FAIL : Grass Mommoth BODY");
    }
    return pGrassMommothBody;
}

void CGrassMommothBody::Free()
{
    __super::Free();
}
