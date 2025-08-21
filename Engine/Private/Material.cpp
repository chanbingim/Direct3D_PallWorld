#include "Material.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext)
{
    Safe_AddRef(pDevice);
    Safe_AddRef(pContext);
}

HRESULT CMaterial::Initialize(const _char* pModelFilePath, const aiMaterial* pAIMaterial)
{
    return S_OK;
}

void CMaterial::SetMaterial(_uInt iMaterialIndex, aiTextureType eType, _uInt iTextureIndex)
{
    if (0 > iTextureIndex || m_iNumSRVs <= iTextureIndex || 0 > iMaterialIndex || 8 <= iMaterialIndex)
        return;

    m_pContext->PSSetShaderResources(iMaterialIndex, 1, &m_SRVs[eType][iTextureIndex]);
}

ID3D11ShaderResourceView* CMaterial::GetMaterial(aiTextureType eType, _uInt iTextureIndex)
{
    if (0 > iTextureIndex || m_iNumSRVs <= iTextureIndex)
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

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
