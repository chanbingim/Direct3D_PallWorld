#include "Material.h"

#include "StringHelper.h"
#include "PaseDataHeader.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CMaterial::Initialize(const _char* pModelFilePath, const aiMaterial* pAIMaterial)
{
    strcpy_s(m_MatrialName, pAIMaterial->GetName().data);

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
            else
                m_SRVs[i].push_back(make_pair(szWideFullPath, pSRV));
        }
    }

    return S_OK;
}

HRESULT CMaterial::Initialize(const _char* pModelFilePath, void* MaterialDesc)
{
    if (nullptr == MaterialDesc)
        return E_FAIL;

    SAVE_MATERIAL_DESC* pMatrialDesc = static_cast<SAVE_MATERIAL_DESC*>(MaterialDesc);

    for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
    {
        MATRIAL_TEXTURE_TYPE_DESC* MatrialDesc = &pMatrialDesc->TextureType[i];

        //해당 타입의 텍스처의 개수를 가져오는 함수이다.
        _uInt m_iNumSRVs = MatrialDesc->iNumShaderResourceView;;
        m_SRVs[i].reserve(m_iNumSRVs);

        for (size_t j = 0; j < m_iNumSRVs; j++)
        {
            /* 추출한파일의경로 + 파일이름 + 확장자 */
            _char		szDrive[MAX_PATH] = {};
            _char		szDir[MAX_PATH] = {};
            _char		szFileName[MAX_PATH] = {};
            _char		szEXT[MAX_PATH] = {};

            _char		szFullPath[MAX_PATH] = {};

            CStringHelper::ConvertWideToUTF(MatrialDesc->TexturePath[j].c_str(), szFullPath);
            _splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
            _splitpath_s(szFullPath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

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
                continue;
            else
                m_SRVs[i].push_back(make_pair(szWideFullPath, pSRV));
        }
    }

    return S_OK;
}

void CMaterial::SetMaterial(_uInt iMaterialIndex, aiTextureType eType, _uInt iTextureIndex)
{
    if (0 > iTextureIndex || m_SRVs[eType].size() <= iTextureIndex || 0 > iMaterialIndex || 8 <= iMaterialIndex)
        return;

    m_pContext->PSSetShaderResources(iMaterialIndex, 1, &m_SRVs[eType][iTextureIndex].second);
}

ID3D11ShaderResourceView* CMaterial::GetMaterial(aiTextureType eType, _uInt iTextureIndex)
{
    if (0 > iTextureIndex || m_SRVs[eType].size() <= iTextureIndex)
        return nullptr;

    return m_SRVs[eType][iTextureIndex].second;
}

HRESULT CMaterial::Export(void *pOut)
{
    if (nullptr == pOut)
        return E_FAIL;

    SAVE_MATERIAL_DESC* pMatrialDesc = static_cast<SAVE_MATERIAL_DESC*>(pOut);
    _uInt iTextureTypeCount{};
    pMatrialDesc->iTextureTypeMax = AI_TEXTURE_TYPE_MAX;
    pMatrialDesc->TextureType.reserve(AI_TEXTURE_TYPE_MAX);

    for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
    {
        MATRIAL_TEXTURE_TYPE_DESC Desc = {};
        Desc.iNumShaderResourceView = (_uInt)m_SRVs[i].size();
        Desc.TexturePath.reserve(Desc.iNumShaderResourceView);

        for (auto& pair : m_SRVs[i])
        {
            Desc.TexturePath.push_back(pair.first);
        }

        pMatrialDesc->TextureType.push_back(Desc);
    }

    return S_OK;
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

CMaterial* CMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, void* MaterialDesc)
{
    CMaterial* pMaterial = new CMaterial(pDevice, pContext);
    if (FAILED(pMaterial->Initialize(pModelFilePath, MaterialDesc)))
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
            Safe_Release(SRV.second);
        }
        SRV_Type.clear();
    }

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
