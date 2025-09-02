#include "PlayerLowerBody.h"

#include "GameInstance.h"

CPlayerLowerBody::CPlayerLowerBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CPlayerLowerBody::CPlayerLowerBody(const CPlayerLowerBody& rhs) :
    CPartObject(rhs)
{
}

HRESULT CPlayerLowerBody::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerLowerBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CPlayerLowerBody::Priority_Update(_float fDeletaTime)
{
}

void CPlayerLowerBody::Update(_float fDeletaTime)
{
    m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime, true, "Fiona", "Bip001 Spine1");
}

void CPlayerLowerBody::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayerLowerBody::Render()
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

HRESULT CPlayerLowerBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Fiona_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CPlayerLowerBody* CPlayerLowerBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerLowerBody* pLowerBody = new CPlayerLowerBody(pDevice, pContext);
    if (FAILED(pLowerBody->Initalize_Prototype()))
    {
        Safe_Release(pLowerBody);
        MSG_BOX("CREATE FAIL : PLAYER LOWER BODY");
    }
    return pLowerBody;
}

CGameObject* CPlayerLowerBody::Clone(void* pArg)
{
    CPlayerLowerBody* pLowerBody = new CPlayerLowerBody(*this);
    if (FAILED(pLowerBody->Initialize(pArg)))
    {
        Safe_Release(pLowerBody);
        MSG_BOX("CLONE FAIL : PLAYER LOWER BODY");
    }
    return pLowerBody;
}

void CPlayerLowerBody::Free()
{
    __super::Free();
}
