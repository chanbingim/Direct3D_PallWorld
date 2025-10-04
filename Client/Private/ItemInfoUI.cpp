#include "ItemInfoUI.h"

#include "GameInstance.h"
#include "ItemObject.h"

CItemInfoUI::CItemInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CItemInfoUI::CItemInfoUI(const CItemInfoUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CItemInfoUI::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemInfoUI::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    ITEM_INFO_DESC* pInfoDesc = static_cast<ITEM_INFO_DESC*>(pArg);
    m_pOwner = static_cast<CItemObject *>(pInfoDesc->pParent);
    m_vAwayPoisition = pInfoDesc->vPosition;
    m_szItemName = pInfoDesc->pItemName;

    return S_OK;
}

void CItemInfoUI::Update(_float fDeletaTime)
{
  
}

void CItemInfoUI::Late_Update(_float fDeletaTime)
{
    _float3 vParentPos = m_pOwner->GetTransform()->GetPosition();

    _vector vAwayPoint = XMLoadFloat3(&m_vAwayPoisition);
    _vector vParentPosition = XMLoadFloat3(&vParentPos);
    _float3 CombindPoisition = {};
    XMStoreFloat3(&CombindPoisition, vParentPosition + vAwayPoint);

    auto MatVP = XMLoadFloat4x4(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)) * XMLoadFloat4x4(&m_pGameInstance->GetMatrix(MAT_STATE::ORTHROGRAPHIC));
    XMStoreFloat2(&m_vFontPosition, XMVector3TransformCoord(XMLoadFloat3(&CombindPoisition), MatVP));

    // 투영 스페이스에 있는 위치
    // X : -1 ~ 1  -> 0 ~ 1 로변경해서 스크린 위치를 구한다.
    // Y : 1 ~ -1 -> 0 ~ 1 로 변경
    //m_vFontPosition.x = ( m_vFontPosition.x + 1.f) * g_iHalfWinSizeX;
    //m_vFontPosition.y = (1.f - m_vFontPosition.y) * g_iHalfWinSizeY - 10.f;

    m_vFontPosition = { 100, 100 };

    m_pTransformCom->SetPosition(CombindPoisition);
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CItemInfoUI::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(1);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(m_szItemName.c_str(), {1.f, 1.f, 0.f, 1.f});
    return S_OK;
}

HRESULT CItemInfoUI::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pEVCamPos = m_pShaderCom->GetVariable("g_vCamPosition");
    return S_OK;
}

HRESULT CItemInfoUI::Apply_ConstantShaderResources()
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));


    _float3 vCamPos = {};
    XMStoreFloat3(&vCamPos, m_pGameInstance->GetCameraState(WORLDSTATE::POSITION));
    m_pEVCamPos->SetRawValue(reinterpret_cast<const float*>(&vCamPos), 0, sizeof(_float3));

    return S_OK;
}

HRESULT CItemInfoUI::ADD_Components()
{
    CFontComponent::FONT_DESC FontDesc = {};
    FontDesc.pPoint = &m_vFontPosition;
    FontDesc.szUseFontName = TEXT("HanSanFont_16");
    FontDesc.vFontSize = { 50, 100 };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Point"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_PellInfo_BackGround_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_WorldUI"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CItemInfoUI* CItemInfoUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItemInfoUI* pItemInfoUI = new CItemInfoUI(pDevice, pContext);
    if (FAILED(pItemInfoUI->Initalize_Prototype()))
    {
        Safe_Release(pItemInfoUI);
        MSG_BOX("CREATE FAIL : ITEM INFO UI");
    }
    return pItemInfoUI;
}

CGameObject* CItemInfoUI::Clone(void* pArg)
{
    CItemInfoUI* pItemInfoUI = new CItemInfoUI(*this);
    if (FAILED(pItemInfoUI->Initialize(pArg)))
    {
        Safe_Release(pItemInfoUI);
        MSG_BOX("CLONE FAIL : ITEM INFO UI");
    }
    return pItemInfoUI;
}

void CItemInfoUI::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
