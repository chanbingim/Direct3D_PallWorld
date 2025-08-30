#include "TreeObject.h"

#include "GameInstance.h"

CTreeObject::CTreeObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CEnviormnent(pDevice, pContext)
{
}

CTreeObject::CTreeObject(const CTreeObject& rhs) : 
    CEnviormnent(rhs)
{
}

HRESULT CTreeObject::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTreeObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CTreeObject::Priority_Update(_float fDeletaTime)
{
}

void CTreeObject::Update(_float fDeletaTime)
{
}

void CTreeObject::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CTreeObject::Render()
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

HRESULT CTreeObject::ADD_Components()
{
    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT(""), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;
    
    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CTreeObject* CTreeObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTreeObject* pTreeObject = new CTreeObject(pDevice, pContext);
    if (FAILED(pTreeObject->Initalize_Prototype()))
    {
        Safe_Release(pTreeObject);
        MSG_BOX("CREATE FAIL : TREE OBJECT");
    }
    return pTreeObject;
}

CGameObject* CTreeObject::Clone(void* pArg)
{
    CTreeObject* pTreeObject = new CTreeObject(*this);
    if (FAILED(pTreeObject->Initialize(pArg)))
    {
        Safe_Release(pTreeObject);
        MSG_BOX("CLONE FAIL : TREE OBJECT");
    }
    return pTreeObject;
}

void CTreeObject::Free()
{
    __super::Free();
}
