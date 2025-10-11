#include "BossTitleName.h"

#include "GameInstance.h"
#include "TypeIcon.h"

CBossTitleName::CBossTitleName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{
}

CBossTitleName::CBossTitleName(const CBossTitleName& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CBossTitleName::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;
    return S_OK;
}

HRESULT CBossTitleName::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CBossTitleName::Update(_float fDeletaTime)
{
    m_pTypeIcon->Update(fDeletaTime);
}

void CBossTitleName::Late_Update(_float fDeletaTime)
{
    m_pTypeIcon->Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CBossTitleName::Render()
{
    m_pFontCom->Render(m_szText.c_str(), { 1.f, 1.f, 1.f, 1.f });
    return S_OK;
}

HRESULT CBossTitleName::ADD_Components()
{
    // 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
    CFontComponent::FONT_DESC FontDesc = {};
    m_TextFontPos = { (_float)GetRectSize().left, (_float)GetRectSize().top };
    FontDesc.pPoint = &m_TextFontPos;
    FontDesc.szUseFontName = TEXT("HanSanFont_16");
    FontDesc.vFontSize = { 50, 100 };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
        return E_FAIL;

    CTypeIcon::GAMEOBJECT_DESC IconDesc = {};
    IconDesc.pParent = this;
    IconDesc.vScale = { 0.5f, 0.5f, 0.f };
    IconDesc.vPosition = {};

    m_pTypeIcon = static_cast<CTypeIcon*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Netrual_Pell_Type_UI"), &IconDesc));
    m_pTypeIcon->SetZOrder(m_iZOrder + 1);
    ADD_Child(m_pTypeIcon);

    return S_OK;
}

CBossTitleName* CBossTitleName::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBossTitleName* pBossTitleName = new CBossTitleName(pDevice, pContext);
    if (FAILED(pBossTitleName->Initalize_Prototype()))
    {
        Safe_Release(pBossTitleName);
        MSG_BOX("CREATE FAIL : BOSS TITLE NAME");
    }
    return pBossTitleName;
}

CGameObject* CBossTitleName::Clone(void* pArg)
{
    CBossTitleName* pBossTitleName = new CBossTitleName(*this);
    if (FAILED(pBossTitleName->Initialize(pArg)))
    {
        Safe_Release(pBossTitleName);
        MSG_BOX("CLONE FAIL : BOSS TITLE NAME");
    }
    return pBossTitleName;
}

void CBossTitleName::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
