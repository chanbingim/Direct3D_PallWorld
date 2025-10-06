#include "PlayerBody.h"

#include "GameInstance.h"
#include "PlayerManager.h"

CPlayerBody::CPlayerBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext),
    m_pPlayerManager(CPlayerManager::GetInstance())
{
    Safe_AddRef(m_pPlayerManager);
}

CPlayerBody::CPlayerBody(const CPlayerBody& rhs) :
    CPartObject(rhs),
    m_pPlayerManager(CPlayerManager::GetInstance())
{
    Safe_AddRef(m_pPlayerManager);
}

HRESULT CPlayerBody::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CPlayerBody::Priority_Update(_float fDeletaTime)
{
 
}

void CPlayerBody::Update(_float fDeletaTime)
{
    
}

void CPlayerBody::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();
}

HRESULT CPlayerBody::Render()
{
    if (nullptr == m_pClothesBuffer)
    {
        _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
        for (_uInt i = 0; i < iNumMeshes; ++i)
        {
            Apply_ConstantShaderResources(i);
            m_pShaderCom->Update_Shader(0);
            m_pVIBufferCom->Render(i);
        }
    }
    else
    {
        _uInt iNumMeshes = m_pClothesBuffer->GetNumMeshes();
        for (_uInt i = 0; i < iNumMeshes; ++i)
        {
            Apply_ConstantShaderResources(i);
            m_pShaderCom->Update_Shader(0);
            m_pClothesBuffer->Render(i);
        }
    }
    return S_OK;
}

void CPlayerBody::UpdateAnimation(CModel* pVIBuffer)
{
    m_pClothesBuffer = m_pPlayerManager->GetCurrentSlotItemModel(EUQIP_TYPE::BODY);

    if (nullptr == m_pClothesBuffer)
        m_pVIBufferCom->BindParentAnim(pVIBuffer);
    else
        m_pClothesBuffer->BindParentAnim(pVIBuffer);
}

HRESULT CPlayerBody::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedWorldMatrix));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    ID3D11ShaderResourceView* pResourceVeiw = {};
    ID3D11ShaderResourceView* pResourceNormalVeiw = {};
    if (nullptr == m_pClothesBuffer)
    {
        m_pBoneMatrixEffect->SetMatrixArray(reinterpret_cast<const float*>(m_pVIBufferCom->GetBoneMatrices(iMeshIndex)), 0, m_pVIBufferCom->GetMeshNumBones(iMeshIndex));
        m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
        m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_NORMALS, 0, &pResourceNormalVeiw);
    }
    else
    {
        m_pBoneMatrixEffect->SetMatrixArray(reinterpret_cast<const float*>(m_pClothesBuffer->GetBoneMatrices(iMeshIndex)), 0, m_pClothesBuffer->GetMeshNumBones(iMeshIndex));
        m_pClothesBuffer->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
        m_pClothesBuffer->GetMeshResource(iMeshIndex, aiTextureType_NORMALS, 0, &pResourceNormalVeiw);
        
    }
      
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);

    if(pResourceNormalVeiw)
        m_pShaderCom->Bind_SRV("g_NormalTexture", pResourceNormalVeiw);

    return S_OK;
}

HRESULT CPlayerBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_OldClothes"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CPlayerBody* CPlayerBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerBody* pUpperBody = new CPlayerBody(pDevice, pContext);
    if (FAILED(pUpperBody->Initalize_Prototype()))
    {
        Safe_Release(pUpperBody);
        MSG_BOX("CREATE FAIL : PLAYER UPPER BODY");
    }
    return pUpperBody;
}

CGameObject* CPlayerBody::Clone(void* pArg)
{
    CPlayerBody* pUpperBody = new CPlayerBody(*this);
    if (FAILED(pUpperBody->Initialize(pArg)))
    {
        Safe_Release(pUpperBody);
        MSG_BOX("CLONE FAIL : PLAYER UPPER BODY");
    }
    return pUpperBody;
}

void CPlayerBody::Free()
{
    __super::Free();

    Safe_Release(m_pPlayerManager);
}
