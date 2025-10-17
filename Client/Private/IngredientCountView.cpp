#include "IngredientCountView.h"

#include "GameInstance.h"

CIngredientCountView::CIngredientCountView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CIngredientCountView::CIngredientCountView(const CIngredientCountView& rhs) :
    CBackGround(rhs)
{
}

HRESULT CIngredientCountView::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;
    return S_OK;
}

HRESULT CIngredientCountView::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CIngredientCountView::Update(_float fDeletaTime)
{
}

void CIngredientCountView::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CIngredientCountView::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(m_szText.c_str(), { 0.f, 0.f, 0.f, 1.f });

    return S_OK;
}

void CIngredientCountView::SetText(const WCHAR* szText)
{
    m_szText = szText;
}

HRESULT CIngredientCountView::ADD_Components()
{
    // 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
    CFontComponent::FONT_DESC FontDesc = {};
    m_vTitleFontPos = { (_float)GetRectSize().left, (_float)GetRectSize().top };
    FontDesc.pPoint = &m_vTitleFontPos;
    FontDesc.szUseFontName = TEXT("HanSanFont_16");
    FontDesc.vFontSize = { 50, 100 };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellInfo_Background"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;
    return S_OK;
}

CIngredientCountView* CIngredientCountView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CIngredientCountView* pIngredientCountView = new CIngredientCountView(pDevice, pContext);
    if (FAILED(pIngredientCountView->Initalize_Prototype()))
    {
        Safe_Release(pIngredientCountView);
        MSG_BOX("CREATE FAIL : INGREDIENT COUNT VIEW");
    }
    return pIngredientCountView;
}

CGameObject* CIngredientCountView::Clone(void* pArg)
{
    CIngredientCountView* pIngredientCountView = new CIngredientCountView(*this);
    if (FAILED(pIngredientCountView->Initialize(pArg)))
    {
        Safe_Release(pIngredientCountView);
        MSG_BOX("CLONE FAIL : INGREDIENT COUNT VIEW");
    }
    return pIngredientCountView;
}

void CIngredientCountView::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
