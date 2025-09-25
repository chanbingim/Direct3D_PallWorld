#include "InstanceModel.h"

#include "GameInstance.h"

CInstanceModel::CInstanceModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CGameObject(pDevice, pContext)
{
}

CInstanceModel::CInstanceModel(const CInstanceModel& rhs) :
    CGameObject(rhs)
{
}

HRESULT CInstanceModel::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CInstanceModel::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CInstanceModel::Priority_Update(_float fDeletaTime)
{
}

void CInstanceModel::Update(_float fDeletaTime)
{
}

void CInstanceModel::Late_Update(_float fDeletaTime)
{
}

HRESULT CInstanceModel::Render()
{
    return E_NOTIMPL;
}

HRESULT CInstanceModel::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();

    return S_OK;
}

HRESULT CInstanceModel::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    ID3D11ShaderResourceView* pResourceVeiw = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);

    return S_OK;
}


CGameObject* CInstanceModel::Clone(void* pArg)
{
    return nullptr;
}

void CInstanceModel::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
}
