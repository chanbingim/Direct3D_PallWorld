#include "Compass.h"

#include "GameInstance.h"
#include "GamePlayHUD.h"

CCompass::CCompass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CCompass::CCompass(const CCompass& rhs) :
    CBackGround(rhs)
{
}

HRESULT CCompass::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CCompass::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_iZOrder = 0;
    m_eType = OBJECT_TYPE::STATIC;
    return S_OK;
}

void CCompass::Update(_float fDeletaTime)
{
    WCHAR Debug_Log[MAX_PATH] = {};

    _vector vLook = XMVector3Normalize(m_pGameInstance->GetCameraState(WORLDSTATE::LOOK));
    vLook.m128_f32[1] = 0.f;
    m_fRadius = atan2f(XMVectorGetX(vLook), XMVectorGetZ(vLook));
    if (m_fRadius < 0)
        m_fRadius += XM_2PI;
}

void CCompass::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CCompass::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(0);

    m_pTextureCom->SetTexture(0, 0);
    m_pTextureCom->SetTexture(1, 1);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

void CCompass::SetCompass(_float fRadius)
{
    m_fRadius = fRadius;
}

HRESULT CCompass::Bind_ShaderResources()
{
    if(FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    m_pUvPercent =  m_pShaderCom->GetVariable("g_Percent");
    return S_OK;
}

HRESULT CCompass::Apply_ConstantShaderResources()
{
    __super::Apply_ConstantShaderResources();

    _float RadiusPercnet = XMConvertToDegrees(m_fRadius) / 360.f;
    m_pUvPercent->SetRawValue(&RadiusPercnet, 0, sizeof(_float));

    return S_OK;
}

HRESULT CCompass::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Compass_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_MutiplyBlend"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CCompass::ADD_Childs()
{
    auto pDefaultHUD = m_pGameInstance->GetCurrentHUD();
    if (nullptr == pDefaultHUD)
        return E_FAIL;

    auto pInGame_HUD = dynamic_cast<CGamePlayHUD*>(pDefaultHUD);
    if (pInGame_HUD)
    {
        CBackGround::GAMEOBJECT_DESC Desc = {};
        Desc.pParent = this;
        Desc.vScale = { 15.f, 15.f, 0.f };

        //Guard bar
        Desc.vPosition = { 0.f, 13.f, 0.f };
        if (FAILED(pInGame_HUD->Add_UserInterface(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Compass_Point"), TEXT("CompassIcon"), &Desc)))
            return E_FAIL;
    }
    return S_OK;
}

CCompass* CCompass::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCompass* pCompass = new CCompass(pDevice, pContext);
    if (FAILED(pCompass->Initalize_Prototype()))
    {
        Safe_Release(pCompass);
        MSG_BOX("CREATE FAIL : COMPASS");
    }

    return pCompass;
}

CGameObject* CCompass::Clone(void* pArg)
{
    CCompass* pCompass = new CCompass(*this);
    if (FAILED(pCompass->Initialize(pArg)))
    {
        Safe_Release(pCompass);
        MSG_BOX("CLONE FAIL : COMPASS");
    }

    return pCompass;
}

void CCompass::Free()
{
    __super::Free();
}
