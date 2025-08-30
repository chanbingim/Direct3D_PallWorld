#include "RockObject.h"

#include "GameInstance.h"

CRockObject::CRockObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CEnviormnent(pDevice, pContext)
{
}

CRockObject::CRockObject(const CRockObject& rhs) :
    CEnviormnent(rhs)
{
}

HRESULT CRockObject::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CRockObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    ENVIORMNENT_DESC* pEnvDesc = static_cast<ENVIORMNENT_DESC*>(pArg);
    if (FAILED(ADD_Components(pEnvDesc->iModelIndex)))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CRockObject::Priority_Update(_float fDeletaTime)
{
}

void CRockObject::Update(_float fDeletaTime)
{
}

void CRockObject::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CRockObject::Render()
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

HRESULT CRockObject::ADD_Components(_uInt iModelIndex)
{
    WCHAR szModelName[MAX_PATH] = {};
    wsprintf(szModelName, TEXT("Prototype_Component_VIBuffer_Rock_%d_Mesh"), iModelIndex);

    // µ¹  ¸ðµ¨ Á¤º¸
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), szModelName, TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CRockObject* CRockObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CRockObject* pRockObject = new CRockObject(pDevice, pContext);
    if (FAILED(pRockObject->Initalize_Prototype()))
    {
        Safe_Release(pRockObject);
        MSG_BOX("CREATE FAIL : ROCK OBJECT");
    }
    return pRockObject;
}

CGameObject* CRockObject::Clone(void* pArg)
{
    CRockObject* pRockObject = new CRockObject(*this);
    if (FAILED(pRockObject->Initialize(pArg)))
    {
        Safe_Release(pRockObject);
        MSG_BOX("CLONE FAIL : ROCK OBJECT");
    }
    return pRockObject;
}

void CRockObject::Free()
{
    __super::Free();
}