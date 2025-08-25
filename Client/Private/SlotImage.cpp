#include "SlotImage.h"

#include "GameInstance.h"

CSlotImage::CSlotImage(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CBackGround(pGraphic_Device, pDeviceContext)
{
}

CSlotImage::CSlotImage(const CSlotImage& rhs) :
    CBackGround(rhs)
{
}

HRESULT CSlotImage::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSlotImage::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CSlotImage::Update(_float fDeletaTime)
{
}

void CSlotImage::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CSlotImage::Render()
{
    if (m_pTextureCom)
    {
        if (FAILED(Apply_ConstantShaderResources()))
            return E_FAIL;

        m_pShaderCom->Update_Shader(0);

        m_pTextureCom->SetTexture(0, 0);
        m_pVIBufferCom->Render_VIBuffer();
    }
        
    return S_OK;
}

void CSlotImage::ChangeTextureData(CTexture* pTexture)
{
    if (nullptr == pTexture || m_pTextureCom == pTexture)
        return;

    if (m_pTextureCom)
        Safe_Release(m_pTextureCom);

    m_pTextureCom = pTexture;
    Safe_AddRef(m_pTextureCom);
}

HRESULT CSlotImage::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CSlotImage* CSlotImage::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CSlotImage* pSlotImage = new CSlotImage(pGraphic_Device, pDeviceContext);
    if (FAILED(pSlotImage->Initalize_Prototype()))
    {
        Safe_Release(pSlotImage);
        MSG_BOX("CREATE FAIL : SLOT IMAGE");
    }
    return pSlotImage;
}

CGameObject* CSlotImage::Clone(void* pArg)
{
    CSlotImage* pSlotImage = new CSlotImage(*this);
    if (FAILED(pSlotImage->Initialize(pArg)))
    {
        Safe_Release(pSlotImage);
        MSG_BOX("CLONE FAIL : SLOT IMAGE");
    }
    return pSlotImage;
}

void CSlotImage::Free()
{
    __super::Free();

}
