#include "Material.h"

#include "StringHelper.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CMaterial::Initialize(const _char* pModelFilePath, const aiMaterial* pAIMaterial)
{
    for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
    {
        //해당 타입의 텍스처의 개수를 가져오는 함수이다.
        _uInt m_iNumSRVs = pAIMaterial->GetTextureCount(static_cast<aiTextureType>(i));
        m_SRVs[i].reserve(m_iNumSRVs);

        for (size_t j = 0; j < m_iNumSRVs; j++)
        {
            /* 추출한파일의경로 + 파일이름 + 확장자 */
            aiString	strTexturePath = {};

            if (FAILED(pAIMaterial->GetTexture(static_cast<aiTextureType>(i), (_uInt)j, &strTexturePath)))
                continue;

            _char		szDrive[MAX_PATH] = {};
            _char		szDir[MAX_PATH] = {};
            _char		szFileName[MAX_PATH] = {};
            _char		szEXT[MAX_PATH] = {};

            _char		szFullPath[MAX_PATH] = {};
            _splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
            _splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

            sprintf_s(szFullPath, MAX_PATH, "%s%s%s%s", szDrive, szDir, szFileName, szEXT);

            _tchar		szWideFullPath[MAX_PATH] = {};
            CStringHelper::ConvertUTFToWide(szFullPath, szWideFullPath);

            ID3D11ShaderResourceView* pSRV = { nullptr };
            HRESULT			hr = {};

            if (false == strcmp(".dds", szEXT))
                hr = CreateDDSTextureFromFile(m_pDevice, szWideFullPath, nullptr, &pSRV);
            else if (false == strcmp(".tga", szEXT))
                hr = S_OK;
            else
                hr = CreateWICTextureFromFile(m_pDevice, szWideFullPath, nullptr, &pSRV);

            if (FAILED(hr))
                return E_FAIL;

            m_SRVs[i].push_back(pSRV);
        }
    }

    return S_OK;
}

void CMaterial::SetMaterial(_uInt iMaterialIndex, aiTextureType eType, _uInt iTextureIndex)
{
    if (0 > iTextureIndex || m_SRVs[eType].size() <= iTextureIndex || 0 > iMaterialIndex || 8 <= iMaterialIndex)
        return;

    m_pContext->PSSetShaderResources(iMaterialIndex, 1, &m_SRVs[eType][iTextureIndex]);
}

ID3D11ShaderResourceView* CMaterial::GetMaterial(aiTextureType eType, _uInt iTextureIndex)
{
    if (0 > iTextureIndex || m_SRVs[eType].size() <= iTextureIndex)
        return nullptr;

    return m_SRVs[eType][iTextureIndex];
}

CMaterial* CMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, const aiMaterial* pAIMaterial)
{
    CMaterial* pMaterial = new CMaterial(pDevice, pContext);
    if (FAILED(pMaterial->Initialize(pModelFilePath, pAIMaterial)))
    {
        Safe_Release(pMaterial);
        MSG_BOX("CREATE FAIL : MATERIAL");
    }
    return pMaterial;
}

void CMaterial::Free()
{
    __super::Free();

    for(auto SRV_Type : m_SRVs)
    {
        for (auto SRV : SRV_Type)
        {
            Safe_Release(SRV);
        }
        SRV_Type.clear();
    }

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
