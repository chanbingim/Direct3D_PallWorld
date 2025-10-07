#include "ActionAbleUI.h"

#include "GameInstance.h"

CActionAbleUI::CActionAbleUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CActionAbleUI::CActionAbleUI(const CActionAbleUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CActionAbleUI::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CActionAbleUI::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;


    ACTION_ABLE_DESC* pActionDesc = static_cast<ACTION_ABLE_DESC*>(pArg);
    m_pOwner = pActionDesc->pOwner;
    m_vAwayPosition = pActionDesc->vAwayPosition;

    return S_OK;
}

void CActionAbleUI::Update(_float fDeletaTime)
{
    _float3 vParentPos = m_pOwner->GetTransform()->GetPosition();

    _vector vAwayPoint = XMLoadFloat3(&m_vAwayPosition);
    _vector vParentPosition = XMLoadFloat3(&vParentPos);
    _float3 CombindPoisition = {};
    XMStoreFloat3(&CombindPoisition, vParentPosition + vAwayPoint);

    auto MatVP = XMLoadFloat4x4(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)) * XMLoadFloat4x4(&m_pGameInstance->GetMatrix(MAT_STATE::ORTHROGRAPHIC));
    XMStoreFloat2(&m_vFontPos, XMVector3TransformCoord(XMLoadFloat3(&CombindPoisition), MatVP));

    // 투영 스페이스에 있는 위치
    // X : -1 ~ 1  -> 0 ~ 1 로변경해서 스크린 위치를 구한다.
    // Y : 1 ~ -1 -> 0 ~ 1 로 변경
    m_vFontPos.x = (m_vFontPos.x + 1.f) * g_iHalfWinSizeX;
    m_vFontPos.y = (1.f - m_vFontPos.y) * g_iHalfWinSizeY - 10.f;

    m_pTransformCom->SetPosition(CombindPoisition);
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

void CActionAbleUI::Late_Update(_float fDeletaTime)
{
    
}

HRESULT CActionAbleUI::Render()
{
    Apply_ConstantShaderResources();

    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(TEXT("상호 작용"), {0.f,0.f,0.f,1.f});
    return S_OK;
}

HRESULT CActionAbleUI::Apply_ConstantShaderResources()
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    _float4 vCamPos = {};
    XMStoreFloat4(&vCamPos, m_pGameInstance->GetCameraState(WORLDSTATE::POSITION));
    m_pShaderCom->Bind_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4));

    return S_OK;
}

HRESULT CActionAbleUI::ADD_Components()
{
    // 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
    CFontComponent::FONT_DESC FontDesc = {};
    m_vFontPos = { (_float)GetRectSize().left, (_float)GetRectSize().top };
    FontDesc.pPoint = &m_vFontPos;
    FontDesc.szUseFontName = TEXT("HanSanFont_16");
    FontDesc.vFontSize = { 50, 100 };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
        return E_FAIL;


    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Slot_Base_Texture"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_WorldUI"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;


    return S_OK;
}

CActionAbleUI* CActionAbleUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CActionAbleUI* pActionAble = new CActionAbleUI(pDevice, pContext);
    if (FAILED(pActionAble->Initalize_Prototype()))
    {
        Safe_Release(pActionAble);
        MSG_BOX("CREATE FAIL : ACTION ABLE UI");
    }
    return pActionAble;
}

CUserInterface* CActionAbleUI::Clone(void* pArg)
{
    CActionAbleUI* pActionAble = new CActionAbleUI(*this);
    if (FAILED(pActionAble->Initialize(pArg)))
    {
        Safe_Release(pActionAble);
        MSG_BOX("CLONE FAIL : ACTION ABLE UI");
    }
    return pActionAble;
}

void CActionAbleUI::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
