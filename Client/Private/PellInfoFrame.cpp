#include "PellInfoFrame.h"

#include "GameInstance.h"

CPellInfoFrame::CPellInfoFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CPellInfoFrame::CPellInfoFrame(const CPellInfoFrame& rhs) :
    CBackGround(rhs)
{
}

HRESULT CPellInfoFrame::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPellInfoFrame::Initialize(void* pArg)
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

void CPellInfoFrame::Update(_float fDeletaTime)
{
}

void CPellInfoFrame::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CPellInfoFrame::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CPellInfoFrame::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellFrame_UI"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CPellInfoFrame* CPellInfoFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPellInfoFrame* PellInfoFrame = new CPellInfoFrame(pDevice, pContext);
    if (FAILED(PellInfoFrame->Initalize_Prototype()))
    {
        Safe_Release(PellInfoFrame);
        MSG_BOX("CREATE FAIL : Pell Info Frame");
    }

    return PellInfoFrame;
}

CGameObject* CPellInfoFrame::Clone(void* pArg)
{
    CPellInfoFrame* PellInfoFrame = new CPellInfoFrame(*this);
    if (FAILED(PellInfoFrame->Initialize(pArg)))
    {
        Safe_Release(PellInfoFrame);
        MSG_BOX("CLONE FAIL : Pell Info Frame");
    }

    return PellInfoFrame;
}

void CPellInfoFrame::Free()
{
    __super::Free();
}
