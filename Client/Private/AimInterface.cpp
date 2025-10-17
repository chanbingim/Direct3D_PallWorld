#include "AimInterface.h"

#include "GameInstance.h"

#include "PlayerManager.h"

CAimInterface::CAimInterface(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CAimInterface::CAimInterface(const CAimInterface& rhs) :
    CBackGround(rhs)
{
}

HRESULT CAimInterface::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CAimInterface::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    m_iZOrder = 0;
    m_eType = OBJECT_TYPE::STATIC;
    m_eVisible = VISIBILITY::HIDDEN;

    return S_OK;
}

void CAimInterface::Update(_float fDeletaTime)
{

}

void CAimInterface::Late_Update(_float fDeletaTime)
{
    if(CPlayerManager::GetInstance()->IsAimState())
        m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CAimInterface::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);

    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CAimInterface::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_CrossHair_Defualt"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CAimInterface* CAimInterface::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CAimInterface* pAimInterface = new CAimInterface(pGraphic_Device, pDeviceContext);
    if (FAILED(pAimInterface->Initalize_Prototype()))
    {
        Safe_Release(pAimInterface);
        MSG_BOX("CREATE FAIL : AimInterface");
    }

    return pAimInterface;
}

CGameObject* CAimInterface::Clone(void* pArg)
{
    CAimInterface* pAimInterface = new CAimInterface(*this);
    if (FAILED(pAimInterface->Initialize(pArg)))
    {
        Safe_Release(pAimInterface);
        MSG_BOX("CLONE FAIL : AimInterface");
    }

    return pAimInterface;
}

void CAimInterface::Free()
{
    __super::Free();
}
