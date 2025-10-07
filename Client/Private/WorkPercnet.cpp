#include "WorkPercnet.h"

#include "GameInstance.h"
#include "WorkPercentBar.h"

CWorkPercnet::CWorkPercnet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CWorkPercnet::CWorkPercnet(const CWorkPercnet& rhs) :
    CBackGround(rhs)
{
}

HRESULT CWorkPercnet::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWorkPercnet::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    return S_OK;
}

void CWorkPercnet::Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CWorkPercnet::Late_Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CWorkPercnet::Render()
{
    Apply_ConstantShaderResources();

    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

HRESULT CWorkPercnet::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Slot_Base_Texture"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_WorldUI"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CWorkPercnet::ADD_Childs()
{
    CWorkPercnet::GAMEOBJECT_DESC PercentDesc = {};
    PercentDesc.pParent = this;
    PercentDesc.vScale = {};
    PercentDesc.vPosition = {};

    m_pWorPercentBar = CWorkPercentBar::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(m_pWorPercentBar->Initialize(&PercentDesc)))
        return E_FAIL;
    ADD_Child(m_pWorPercentBar);

    return S_OK;
}

CWorkPercnet* CWorkPercnet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWorkPercnet* pWorkPercent = new CWorkPercnet(pDevice, pContext);
    if (FAILED(pWorkPercent->Initalize_Prototype()))
    {
        Safe_Release(pWorkPercent);
        MSG_BOX("CREATE FAIL : WORK PERCENT");
    }
    return pWorkPercent;
}

CUserInterface* CWorkPercnet::Clone(void* pArg)
{
    CWorkPercnet* pWorkPercent = new CWorkPercnet(*this);
    if (FAILED(pWorkPercent->Initialize(pArg)))
    {
        Safe_Release(pWorkPercent);
        MSG_BOX("CLONE FAIL : WORK PERCENT");
    }
    return pWorkPercent;
}

void CWorkPercnet::Free()
{
    __super::Free();

    Safe_Release(m_pWorPercentBar);
}
