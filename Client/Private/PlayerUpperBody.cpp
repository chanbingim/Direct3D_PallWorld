#include "PlayerUpperBody.h"

#include "GameInstance.h"

CPlayerUpperBody::CPlayerUpperBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CPlayerUpperBody::CPlayerUpperBody(const CPlayerUpperBody& rhs) :
    CPartObject(rhs)
{
}

HRESULT CPlayerUpperBody::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerUpperBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CPlayerUpperBody::Priority_Update(_float fDeletaTime)
{

}

void CPlayerUpperBody::Update(_float fDeletaTime)
{
    m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime, true, "Bip001 Spine1");
}

void CPlayerUpperBody::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayerUpperBody::Render()
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

HRESULT CPlayerUpperBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Fiona_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CPlayerUpperBody* CPlayerUpperBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerUpperBody* pUpperBody = new CPlayerUpperBody(pDevice, pContext);
    if (FAILED(pUpperBody->Initalize_Prototype()))
    {
        Safe_Release(pUpperBody);
        MSG_BOX("CREATE FAIL : PLAYER UPPER BODY");
    }
    return pUpperBody;
}

CGameObject* CPlayerUpperBody::Clone(void* pArg)
{
    CPlayerUpperBody* pUpperBody = new CPlayerUpperBody(*this);
    if (FAILED(pUpperBody->Initialize(pArg)))
    {
        Safe_Release(pUpperBody);
        MSG_BOX("CLONE FAIL : PLAYER UPPER BODY");
    }
    return pUpperBody;
}

void CPlayerUpperBody::Free()
{
    __super::Free();
}
