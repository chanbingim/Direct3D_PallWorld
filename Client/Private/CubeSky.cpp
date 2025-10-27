#include "CubeSky.h"

#include "GameInstance.h"

CCubeSky::CCubeSky(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CGameObject(pDevice, pContext)
{
}

CCubeSky::CCubeSky(const CCubeSky& Prototype) :
    CGameObject(Prototype)
{
}

HRESULT CCubeSky::Initalize_Prototype()
{

    return S_OK;
}

HRESULT CCubeSky::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CCubeSky::Priority_Update(_float fDeletaTime)
{
}

void CCubeSky::Update(_float fDeletaTime)
{
    _float3 vCamPos = {};
    XMStoreFloat3(&vCamPos, m_pGameInstance->GetCameraState(WORLDSTATE::POSITION));
    m_pTransformCom->SetPosition(vCamPos);
}

void CCubeSky::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::PRIORITY, this);
}

HRESULT CCubeSky::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Update_Shader(0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CCubeSky::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Sky_Cube"), TEXT("TextureCom"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Sky"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CCubeSky::Bind_ShaderResources()
{
    m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->GetWorldMat());
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION));

    ID3D11ShaderResourceView* pTexSRV = nullptr;
    pTexSRV = m_pTextureCom->GetTexture(0);
    if (pTexSRV)
        m_pShaderCom->Bind_SRV("g_Texture", pTexSRV);

    return S_OK;
}

CCubeSky* CCubeSky::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCubeSky* pCubeSky = new CCubeSky(pDevice, pContext);
    if (FAILED(pCubeSky->Initalize_Prototype()))
    {
        Safe_Release(pCubeSky);
        MSG_BOX("CREATE FAIL : CUBE SKY");
    }
    return pCubeSky;
}

CGameObject* CCubeSky::Clone(void* pArg)
{
    CCubeSky* pCubeSky = new CCubeSky(*this);
    if (FAILED(pCubeSky->Initialize(pArg)))
    {
        Safe_Release(pCubeSky);
        MSG_BOX("CLONE FAIL : CUBE SKY");
    }
    return pCubeSky;
}

void CCubeSky::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
}
