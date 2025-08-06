#include "TypeIcon.h"

#include "GameInstance.h"

CTypeIcon::CTypeIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CTypeIcon::CTypeIcon(const CTypeIcon& rhs) :
    CBackGround(rhs)
{
}

HRESULT CTypeIcon::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTypeIcon::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    m_iZOrder = 1;
    return S_OK;
}

void CTypeIcon::Update(_float fDeletaTime)
{
}

void CTypeIcon::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTypeIcon::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, m_iTypeID);
    m_pVIBufferCom->Render_VIBuffer();

    if (m_pFrontTex)
    {
        m_pTextureCom->SetTexture(0, m_iTypeID);
        m_pVIBufferCom->Render_VIBuffer();
    }

    return S_OK;
}

void CTypeIcon::SetData(CTexture* FrontImage, _uInt iID)
{
    if (m_pFrontTex)
        Safe_Release(m_pFrontTex);

    m_pFrontTex = FrontImage;
    Safe_AddRef(m_pFrontTex);
    m_iTypeID = iID;
}

HRESULT CTypeIcon::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_Pell_TypeUI"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CTypeIcon* CTypeIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTypeIcon* pTpyeIcon = new CTypeIcon(pDevice, pContext);
    if (FAILED(pTpyeIcon->Initalize_Prototype()))
    {
        Safe_Release(pTpyeIcon);
        MSG_BOX("CREATE FAIL : Type Icon");
    }

    return pTpyeIcon;
}

CGameObject* CTypeIcon::Clone(void* pArg)
{
    CTypeIcon* pTpyeIcon = new CTypeIcon(*this);
    if (FAILED(pTpyeIcon->Initialize(pArg)))
    {
        Safe_Release(pTpyeIcon);
        MSG_BOX("CLONE FAIL : Type Icon");
    }

    return pTpyeIcon;
}

void CTypeIcon::Free()
{
    __super::Free();

    Safe_Release(m_pFrontTex);
}