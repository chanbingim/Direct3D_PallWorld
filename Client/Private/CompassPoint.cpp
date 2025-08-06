#include "CompassPoint.h"

#include "GameInstance.h"

CCompassPoint::CCompassPoint(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CBackGround(pGraphic_Device, pDeviceContext)
{
}

CCompassPoint::CCompassPoint(const CCompassPoint& rhs) :
    CBackGround(rhs)
{
}

HRESULT CCompassPoint::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CCompassPoint::Initialize(void* pArg)
{
    if (FAILED(CUserInterface::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    m_iZOrder = 1;
    return S_OK;
}

void CCompassPoint::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CCompassPoint::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
}

HRESULT CCompassPoint::Render()
{
    Apply_ConstantShaderResources();

    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);

    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CCompassPoint::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Compass_Point_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CCompassPoint* CCompassPoint::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CCompassPoint* pCompassPoint = new CCompassPoint(pGraphic_Device, pDeviceContext);
    if (FAILED(pCompassPoint->Initalize_Prototype()))
    {
        Safe_Release(pCompassPoint);
        MSG_BOX("CREATE FAIL : Compass ICon");
    }
    return pCompassPoint;
}

CGameObject* CCompassPoint::Clone(void* pArg)
{
    CCompassPoint* pCompassPoint = new CCompassPoint(*this);
    if (FAILED(pCompassPoint->Initialize(pArg)))
    {
        Safe_Release(pCompassPoint);
        MSG_BOX("CLONE FAIL : Compass ICon");
    }
    return pCompassPoint;
}

void CCompassPoint::Free()
{
    __super::Free();
}
