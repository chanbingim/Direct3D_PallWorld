#include "FontComponent.h"

#include "GameInstance.h"

CFontComponent::CFontComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

HRESULT CFontComponent::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CFontComponent::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    FONT_DESC* pDesc = static_cast<FONT_DESC*>(pArg);
    m_pPoint = pDesc->pPoint;
    m_szUseFontTag = pDesc->szUseFontName;
    m_vFontSize = pDesc->vFontSize;

    return S_OK;
}

HRESULT CFontComponent::Render(const _tchar* pText, _vector vColor, _float2 Offset)
{
    return m_pGameInstance->Render_Font(m_szUseFontTag, pText, { m_pPoint->x + Offset.x, m_pPoint->y + Offset.y }, vColor);
}

CFontComponent* CFontComponent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFontComponent* pFont = new CFontComponent(pDevice, pContext);
    if (FAILED(pFont->Initialize_Prototype()))
    {
        Safe_Release(pFont);
        MSG_BOX("CREATE FAIL : FONT COMPONENT");
    }
    return pFont;
}

CComponent* CFontComponent::Clone(void* pArg)
{
    CComponent* pFont = new CFontComponent(*this);
    if (FAILED(pFont->Initialize(pArg)))
    {
        Safe_Release(pFont);
        MSG_BOX("CLONE FAIL : FONT COMPONENT");
    }
    return pFont;
}

void CFontComponent::Free()
{
    __super::Free();
}
