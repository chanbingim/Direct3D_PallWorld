#include "FontManager.h"
#include "CustomFont.h"

CFontManager::CFontManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CFontManager::Add_Font(const _wstring& FontTag, const _tchar* pFontFilePath)
{
    if (nullptr != Find_Font(FontTag))
        return E_FAIL;

    auto pFont = CCustomFont::Create(m_pDevice, m_pContext, pFontFilePath);
    if (nullptr == pFont)
        return E_FAIL;

    m_Fonts.emplace(FontTag, pFont);
    return S_OK;
}

HRESULT CFontManager::Render(const _wstring& FontTag, const _tchar* pText, const _float2& vPosition, _vector vColor)
{
    CCustomFont* pFont = Find_Font(FontTag);
    if (nullptr == pFont)
        return E_FAIL;

    return pFont->Render(pText, vPosition, vColor);
}

_vector CFontManager::GetFontBoundBox(const _wstring& FontTag, const WCHAR* pText)
{
    CCustomFont* pFont = Find_Font(FontTag);
    if (nullptr == pFont)
        return {};

    return pFont->GetFontBoundBox(pText);
}

void CFontManager::GetSpriteSheet(const _wstring& FontTag, ID3D11ShaderResourceView** pText)
{
    CCustomFont* pFont = Find_Font(FontTag);
    if (nullptr == pFont)
        return;

    pFont->GetSpriteSheet(pText);
}

CCustomFont* CFontManager::Find_Font(const _wstring& FontTag)
{
    auto pair = m_Fonts.find(FontTag);
    if (pair == m_Fonts.end())
        return nullptr;
    return pair->second;
}

CFontManager* CFontManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return new CFontManager(pDevice, pContext);
}

void CFontManager::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    for (auto& pair : m_Fonts)
        Safe_Release(pair.second);

    m_Fonts.clear();
}
