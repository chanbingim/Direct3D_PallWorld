#include "ElectricPandaBody.h"

#include "GameInstance.h"

#include "PellBase.h"
#include "CombatComponent.h"

#include "PellSkillManager.h"
#include "SkillObjectBase.h"

CElectricPandaBody::CElectricPandaBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPellBody(pDevice, pContext)
{
}

CElectricPandaBody::CElectricPandaBody(const CElectricPandaBody& rhs) :
    CPellBody(rhs)
{
}

HRESULT CElectricPandaBody::Initalize_Prototype()
{
  

    return S_OK;
}

HRESULT CElectricPandaBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;
     
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_HandMatrix = GetBoneMatrix("hand_l");
    m_pNoiseTexture = m_pGameInstance->GetTextureResource(TEXT("T_Default_Noise.png"));
    m_pVIBufferCom->Bind_KeyFrameFunction("FarSkill_ActionLoop", 1, [this]() { ShootProjecttileObject(); });
    return S_OK;
}

void CElectricPandaBody::Priority_Update(_float fDeletaTime)
{
}

void CElectricPandaBody::Update(_float fDeletaTime)
{
    m_bIsAnimFinished = m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime, 10.f, m_bIsAnimLoop);
}

void CElectricPandaBody::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CElectricPandaBody::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        if (m_bIsDissolve)
        {
            
            m_pShaderCom->Bind_SRV("g_DissolveTexture", m_pNoiseTexture->GetTexture(0));
            m_pShaderCom->Bind_RawValue("g_bIsDissolve", &m_bIsDissolve, sizeof(_bool));
            m_pShaderCom->Bind_RawValue("g_fDissloveTime", &m_fAccDissolveTime, sizeof(_float));
        }

        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render(i);
    }

    return S_OK;
}

HRESULT CElectricPandaBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_ElectricPanda_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CElectricPandaBody::ShootProjecttileObject()
{
    if (-1 == m_iSkillIndex)
        return E_FAIL;

    _float3 vScale = m_pTransformCom->GetScale();
    _float3 vPosition = m_pParent->GetTransform()->GetPosition();

    _vector vLook = m_pTransformCom->GetLookVector();
    _vector vRight = m_pTransformCom->GetRightVector();
    vLook *= vScale.z * 0.5f;

    CPellBase* pOwner = static_cast<CPellBase*>(m_pParent);
    CCombatComponent* pCombatCom = static_cast<CCombatComponent*>(pOwner->Find_Component(TEXT("Combat_Com")));

    _float3 vTargetPoint = pCombatCom->GetCurrentTarget()->GetTransform()->GetPosition();
    _vector vCalPostion = XMLoadFloat3(&vPosition);
    vRight *= m_pGameInstance->Random(3.f, 5.f);

    CSkillObjectBase::SKILL_OBJECT_DESC SkillDesc = {};
    SkillDesc.vScale = { 1.f, 1.f, 1.f };
    SkillDesc.pOwner = static_cast<CPellBase*>(m_pParent);
    //여기서 이펙트 생성해서 넘기기
    if (7 == m_iSkillIndex)
    {
        XMStoreFloat3(&SkillDesc.vTargetDir, XMVector3Normalize(XMLoadFloat3(&vTargetPoint) - XMLoadFloat3(&SkillDesc.vPosition)));
        XMStoreFloat3(&SkillDesc.vPosition, vCalPostion + vLook + vRight);
        SkillDesc.vPosition.y += m_pGameInstance->Random(0.5f, 1.5f);
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricBall"),
            ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Effect"), &SkillDesc)))
            return E_FAIL;

        XMStoreFloat3(&SkillDesc.vPosition, vCalPostion + vLook - vRight);
        XMStoreFloat3(&SkillDesc.vTargetDir, XMVector3Normalize(XMLoadFloat3(&vTargetPoint) - XMLoadFloat3(&SkillDesc.vPosition)));
        SkillDesc.vPosition.y += m_pGameInstance->Random(0.5f, 1.5f);
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricBall"),
            ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Effect"), &SkillDesc)))
            return E_FAIL;
    }
    else if (6 == m_iSkillIndex)
    {
        //이건  소켓 메트릭스 달아서 해야긋당
       /* SkillDesc.vPosition = {};
        SkillDesc.pOwner = static_cast<CPellBase*>(m_pParent);

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ElectricBall"),
            ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_GamePlay_Effect"), &SkillDesc)))
            return E_FAIL;*/
    }
   
    m_iSkillIndex = -1;
    return S_OK;
}

CElectricPandaBody* CElectricPandaBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CElectricPandaBody* pElectPandaBody = new CElectricPandaBody(pDevice, pContext);
    if (FAILED(pElectPandaBody->Initalize_Prototype()))
    {
        Safe_Release(pElectPandaBody);
        MSG_BOX("CREATE FAIL : ElECT PANDA BODY");
    }
    return pElectPandaBody;
}

CGameObject* CElectricPandaBody::Clone(void* pArg)
{
    CElectricPandaBody* pElectPandaBody = new CElectricPandaBody(*this);
    if (FAILED(pElectPandaBody->Initialize(pArg)))
    {
        Safe_Release(pElectPandaBody);
        MSG_BOX("CLONE FAIL : ElECT PANDA BODY");
    }
    return pElectPandaBody;
}

void CElectricPandaBody::Free()
{
    __super::Free();

 
}
