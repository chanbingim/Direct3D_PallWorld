#include "FemalePeopleBody_1.h"

#include "GameInstance.h"

CFemalePeopleBody_1::CFemalePeopleBody_1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CNpcBody(pDevice, pContext)
{
}

CFemalePeopleBody_1::CFemalePeopleBody_1(const CFemalePeopleBody_1& rhs) :
    CNpcBody(rhs)
{
}

HRESULT CFemalePeopleBody_1::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CFemalePeopleBody_1::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CFemalePeopleBody_1::Priority_Update(_float fDeletaTime)
{
}

void CFemalePeopleBody_1::Update(_float fDeletaTime)
{
    m_bIsAnimFinished = m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime, 10.f, m_bIsAnimLoop);
}

void CFemalePeopleBody_1::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CFemalePeopleBody_1::Render()
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

HRESULT CFemalePeopleBody_1::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Female_People_1"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CFemalePeopleBody_1* CFemalePeopleBody_1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFemalePeopleBody_1* pFemalePeopleBody1 = new CFemalePeopleBody_1(pDevice, pContext);
    if (FAILED(pFemalePeopleBody1->Initalize_Prototype()))
    {
        Safe_Release(pFemalePeopleBody1);
        MSG_BOX("CREATE FAIL : FEMALE PEOPLE BODY 1");
    }
    return pFemalePeopleBody1;
}

CGameObject* CFemalePeopleBody_1::Clone(void* pArg)
{
    CFemalePeopleBody_1* pFemalePeopleBody1 = new CFemalePeopleBody_1(*this);
    if (FAILED(pFemalePeopleBody1->Initialize(pArg)))
    {
        Safe_Release(pFemalePeopleBody1);
        MSG_BOX("CLONE FAIL : FEMALE PEOPLE BODY 1");
    }
    return pFemalePeopleBody1;
}

void CFemalePeopleBody_1::Free()
{
    __super::Free();
}
