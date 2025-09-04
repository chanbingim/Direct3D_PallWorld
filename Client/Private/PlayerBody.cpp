#include "PlayerBody.h"
#include "GameInstance.h"

CPlayerBody::CPlayerBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CPlayerBody::CPlayerBody(const CPlayerBody& rhs) :
    CPartObject(rhs)
{
}

HRESULT CPlayerBody::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CPlayerBody::Priority_Update(_float fDeletaTime)
{
 
}

void CPlayerBody::Update(_float fDeletaTime)
{

   
}

void CPlayerBody::Late_Update(_float fDeletaTime)
{
    auto parentModel = static_cast<CModel*>(m_pParent->Find_Component(TEXT("VIBuffer_Com")));
    m_pVIBufferCom->BindParentAnim(parentModel);

    UpdateCombinedMatrix();
}

HRESULT CPlayerBody::Render()
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

HRESULT CPlayerBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Clothes02"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CPlayerBody* CPlayerBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerBody* pUpperBody = new CPlayerBody(pDevice, pContext);
    if (FAILED(pUpperBody->Initalize_Prototype()))
    {
        Safe_Release(pUpperBody);
        MSG_BOX("CREATE FAIL : PLAYER UPPER BODY");
    }
    return pUpperBody;
}

CGameObject* CPlayerBody::Clone(void* pArg)
{
    CPlayerBody* pUpperBody = new CPlayerBody(*this);
    if (FAILED(pUpperBody->Initialize(pArg)))
    {
        Safe_Release(pUpperBody);
        MSG_BOX("CLONE FAIL : PLAYER UPPER BODY");
    }
    return pUpperBody;
}

void CPlayerBody::Free()
{
    __super::Free();
}
