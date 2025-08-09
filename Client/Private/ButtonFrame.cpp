#include "ButtonFrame.h"

#include "GameInstance.h"
#include "DefaultEntity.h"

CButtonFrame::CButtonFrame(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CBackGround(pGraphic_Device, pDeviceContext)
{
}

CButtonFrame::CButtonFrame(const CButtonFrame& rhs) :
    CBackGround(rhs)
{
}

HRESULT CButtonFrame::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CButtonFrame::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Animations()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    SetInitFrameMatrix();
    m_eType = OBJECT_TYPE::STATIC;

    return S_OK;
}

void CButtonFrame::Update(_float fDeletaTime)
{
    if (m_pAnimator)
        m_pAnimator->Update(0);

    _float3 vScale = m_pTransformCom->GetScale();
    _float3 vPos = m_pTransformCom->GetPosition();

    SetFrameCenterPos(0, { vPos.x - (vScale.x * 0.5f), vPos.y + (vScale.y * 0.5f), 0.f, 1.f});
    SetFrameCenterPos(1, { vPos.x + (vScale.x * 0.5f), vPos.y + (vScale.y * 0.5f), 0.f, 1.f });
    SetFrameCenterPos(2, { vPos.x + (vScale.x * 0.5f), vPos.y - (vScale.y * 0.5f), 0.f, 1.f });
    SetFrameCenterPos(3, { vPos.x - (vScale.x * 0.5f), vPos.y - (vScale.y * 0.5f), 0.f, 1.f });
}

void CButtonFrame::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CButtonFrame::Render()
{
    for (size_t i = 0; i < 4; ++i)
    {
        Apply_ConstantShaderResources(i);
        m_pShaderCom->Update_Shader(2);
        m_pTextureCom->SetTexture(0, 0);
        m_pVIBufferCom->Render_VIBuffer();
    }
    return S_OK;
}

void CButtonFrame::ResetButtonFrame()
{
    m_pAnimator->Reset_Animation(0);
}

HRESULT CButtonFrame::Apply_ConstantShaderResources(_uInt iIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pFrameCenter[iIndex]));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_ViewMatrix));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_ProjMatrix));

    return S_OK;
}

HRESULT CButtonFrame::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_ButtonFrame"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animation"), TEXT("Animator_Com"), (CComponent**)&m_pAnimator)))
        return E_FAIL;

    return S_OK;
}

HRESULT CButtonFrame::ADD_Animations()
{
    CDefaultEntity* pDefaultEntity = new CDefaultEntity();
    CDefaultEntity::DEFAULT_DESC Desc;

    Desc.pOwner = this;
    Desc.fFrame = 10.f;
    Desc.fRateTime = 1.0f;
    Desc.bIsLoop = true;

    if (FAILED(pDefaultEntity->Initialize(&Desc)))
        return E_FAIL;

    auto originScale = m_pTransformCom->GetScale();
    pDefaultEntity->SetEnitityState(0, originScale, { originScale.x + 10.f, originScale.y + 10.f, originScale .z});
    pDefaultEntity->SetFlag(0);
    m_pAnimator->ADD_Animation(pDefaultEntity);
    return S_OK;
}

void CButtonFrame::SetFrameCenterPos(_uInt iIndex, _float4 vPos)
{
    memcpy(reinterpret_cast<_float4*>(&m_pFrameCenter[iIndex].m[3]), &vPos, sizeof(_float4));
}

void CButtonFrame::SetInitFrameMatrix()
{
    for (_uInt i = 0; i < 4; ++i)
    {
        _float Angle = i * -90.f;

        auto WorldMat = XMMatrixScaling(10.f, 10.f, 1.f)  * XMMatrixRotationZ(XMConvertToRadians(Angle));
        XMStoreFloat4x4(&m_pFrameCenter[i], WorldMat);
    }
}

CButtonFrame* CButtonFrame::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CButtonFrame* pButtonFrame = new CButtonFrame(pGraphic_Device, pDeviceContext);
    if (FAILED(pButtonFrame->Initalize_Prototype()))
    {
        Safe_Release(pButtonFrame);
        MSG_BOX("CREATE FAIL : BUTTON FRAME");
    }
    return pButtonFrame;
}

CGameObject* CButtonFrame::Clone(void* pArg)
{
    CButtonFrame* pButtonFrame = new CButtonFrame(*this);
    if (FAILED(pButtonFrame->Initialize(pArg)))
    {
        Safe_Release(pButtonFrame);
        MSG_BOX("CLONE FAIL : BUTTON FRAME");
    }
    return pButtonFrame;
}

void CButtonFrame::Free()
{
    __super::Free();
    Safe_Release(m_pAnimator);
}
