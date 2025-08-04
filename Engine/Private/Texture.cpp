#include "Texture.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

CTexture::CTexture(const CTexture& rhs) :
    CComponent(rhs),
    m_pSRVvec(rhs.m_pSRVvec)
{
    for (auto& iter : m_pSRVvec)
        Safe_AddRef(iter);
}

HRESULT CTexture::Initialize_Prototype(const WCHAR* szFilePath, _uInt TextureCnt)
{
    HRESULT hr = {};
    ID3D11ShaderResourceView* newResource = nullptr;

    WCHAR       Extension[MAX_PATH] = {};
    _wsplitpath_s(szFilePath, nullptr, 0, nullptr, 0 ,nullptr, 0, Extension, MAX_PATH);

    WCHAR       fullPath[MAX_PATH] = {};
    for (_uInt i = 0; i < TextureCnt; ++i)
    {
        wsprintf(fullPath, szFilePath, i);

        if (!lstrcmp(Extension, TEXT(".dds")))
            hr = CreateDDSTextureFromFile(m_pDevice, fullPath, nullptr, &newResource);
        else if (!lstrcmp(Extension, TEXT(".tga")))
            hr = E_FAIL;
        else
            hr = CreateWICTextureFromFile(m_pDevice, fullPath, nullptr, &newResource);
        
        if (FAILED(hr))
        {
            MSG_BOX("CREATE FAIL : TEXUTRE");
            return E_FAIL;
        }
         
        m_pSRVvec.push_back(newResource);
    }

    return S_OK;
}

void CTexture::SetTexture(_uInt SlotNum, _uInt TexNum)
{
    if (TexNum >= m_pSRVvec.size() || 0 > TexNum)
        return;

    m_pContext->PSSetShaderResources(SlotNum, 1, &m_pSRVvec[TexNum]);
}

ID3D11ShaderResourceView* CTexture::GetTexture(size_t iTextureIndex)
{
    if (iTextureIndex > m_pSRVvec.size() || 0 > iTextureIndex)
        return nullptr;

    return m_pSRVvec[iTextureIndex];
}

HRESULT CTexture::Initialize(void* pArg)
{
    return S_OK;
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const WCHAR* szFilePath, _uInt TextureCnt)
{
    CTexture* pTexture = new CTexture(pDevice, pContext);
    if (FAILED(pTexture->Initialize_Prototype(szFilePath, TextureCnt)))
    {
        Safe_Release(pTexture);
        MSG_BOX("CREATE FAIL : TEXUTRE COM");
    }

    return pTexture;
}

CComponent* CTexture::Clone(void* pArg)
{
    CTexture* pTexture = new CTexture(*this);
    if (FAILED(pTexture->Initialize(pArg)))
    {
        Safe_Release(pTexture);
        MSG_BOX("CLONE FAIL : TEXUTRE COM");
    }

    return pTexture;
}

void CTexture::Free()
{
    __super::Free();

    for (auto& iter : m_pSRVvec)
        Safe_Release(iter);

    m_pSRVvec.clear();
}
