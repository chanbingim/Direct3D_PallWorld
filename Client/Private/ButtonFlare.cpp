#include "ButtonFlare.h"

#include "GameInstance.h"

_float4 CButtonFlare::m_vColor = { 0.f, 132.f / 255.f, 1.0f, 0.3f };

CButtonFlare::CButtonFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CButtonFlare::CButtonFlare(const CButtonFlare& rhs) :
    CBackGround(rhs)
{
}

HRESULT CButtonFlare::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CButtonFlare::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CButtonFlare::Update(_float fDeletaTime)
{
}

void CButtonFlare::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CButtonFlare::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CButtonFlare::Apply_ConstantShaderResources()
{
    m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->GetWorldMat());
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

    m_pShaderCom->Bind_SRV("g_Texture", m_pTextureCom->GetTexture(2));
    m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4));
    return S_OK;
}

HRESULT CButtonFlare::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_Category_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Button"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CButtonFlare* CButtonFlare::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CButtonFlare* pButtonFlare = new CButtonFlare(pDevice, pContext);
    if (FAILED(pButtonFlare->Initalize_Prototype()))
    {
        Safe_Release(pButtonFlare);
        MSG_BOX("CREATE FAIL : BUTTON FLARE");
    }
    return pButtonFlare;
}

CGameObject* CButtonFlare::Clone(void* pArg)
{
    CButtonFlare* pButtonFlare = new CButtonFlare(*this);
    if (FAILED(pButtonFlare->Initialize(pArg)))
    {
        Safe_Release(pButtonFlare);
        MSG_BOX("CLONE FAIL : BUTTON FLARE");
    }
    return pButtonFlare;
}

void CButtonFlare::Free()
{
    __super::Free();
}
