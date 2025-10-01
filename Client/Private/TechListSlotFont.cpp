#include "TechListSlotFont.h"

#include "GameInstance.h"

CTechListSlotFont::CTechListSlotFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CTechListSlotFont::CTechListSlotFont(const CBackGround& rhs) :
    CBackGround(rhs)
{
}

HRESULT CTechListSlotFont::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTechListSlotFont::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    // 나중에 폰트 배경같은거 생기면 알아서 풀어서 사용해라
    //if (FAILED(Bind_ShaderResources()))
    //    return E_FAIL;

    return S_OK;
}

void CTechListSlotFont::Update(_float fDeletaTime)
{
    _vector vFontBound = m_pFontCom->GetFontBoundBox(m_szSelectItemName.c_str());
    m_vFontPos = { GetScreenPos().x - vFontBound.m128_f32[0] * 0.5f,  GetScreenPos().y };

}

void CTechListSlotFont::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechListSlotFont::Render()
{
    m_pFontCom->Render(m_szSelectItemName.c_str(), { 1.f,1.f,1.f,1.f });

    return S_OK;
}

void CTechListSlotFont::SetText(const WCHAR* szItemName)
{
    m_szSelectItemName = szItemName;
}

HRESULT CTechListSlotFont::ADD_Components()
{
    // 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
    CFontComponent::FONT_DESC FontDesc = {};
    m_vFontPos = GetViewPos();
    FontDesc.pPoint = &m_vFontPos;
    FontDesc.szUseFontName = TEXT("HanSanFont_16");
    FontDesc.vFontSize = { 50, 100 };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
        return E_FAIL;

    return S_OK;
}

CTechListSlotFont* CTechListSlotFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTechListSlotFont* pTechListSlot = new CTechListSlotFont(pDevice, pContext);
    if (FAILED(pTechListSlot->Initalize_Prototype()))
    {
        Safe_Release(pTechListSlot);
        MSG_BOX("CREATE FAIL : TECH LIST SLOT FONT");
    }
    return pTechListSlot;
}

CGameObject* CTechListSlotFont::Clone(void* pArg)
{
    CTechListSlotFont* pTechListSlot = new CTechListSlotFont(*this);
    if (FAILED(pTechListSlot->Initialize(pArg)))
    {
        Safe_Release(pTechListSlot);
        MSG_BOX("CLONE FAIL : TECH LIST SLOT FONT");
    }
    return pTechListSlot;
}

void CTechListSlotFont::Free()
{
    __super::Free();

    Safe_Release(m_pFontCom);
}
