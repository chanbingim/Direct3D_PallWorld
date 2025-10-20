#ifdef _DEBUG
#include "ImgManager.h"
#include "IMG_LandScape.h"
#endif // _DEBUG

#include "DefaultMap.h"
#include "TerrainManager.h"

#include "GameInstance.h"



CDefaultMap::CDefaultMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CNoneAnimMesh(pDevice, pContext)
{
}

CDefaultMap::CDefaultMap(const CDefaultMap& rhs) :
    CNoneAnimMesh(rhs)
{
}

HRESULT CDefaultMap::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CDefaultMap::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_ObejctTag = TEXT("MapObject");
    return S_OK;
}

void CDefaultMap::Priority_Update(_float fDeletaTime)
{
  
}

void CDefaultMap::Update(_float fDeletaTime)
{

}

void CDefaultMap::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CDefaultMap::Render()
{
    _uInt iNum = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNum; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render(i);
    }
    CTerrainManager::GetInstance()->Render();
    
    return S_OK;
}

HRESULT CDefaultMap::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();

    return S_OK;
}

HRESULT CDefaultMap::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    ID3D11ShaderResourceView* pResourceVeiw = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);

    ID3D11ShaderResourceView* pNormalRSV = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_NORMALS, 0, &pNormalRSV);
    if (pNormalRSV)
        m_pShaderCom->Bind_SRV("g_NormalTexture", pNormalRSV);

    return S_OK;
}

HRESULT CDefaultMap::ADD_Components()
{
    // µ¹  ¸ðµ¨ Á¤º¸
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_DefaultMap"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CDefaultMap* CDefaultMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDefaultMap* pDefalutMap = new CDefaultMap(pDevice, pContext);
    if (FAILED(pDefalutMap->Initalize_Prototype()))
    {
        Safe_Release(pDefalutMap);
        MSG_BOX("CREATE FAIL : DEFAUTL MAP");
    }
    return pDefalutMap;
}

CGameObject* CDefaultMap::Clone(void* pArg)
{
    CDefaultMap* pDefalutMap = new CDefaultMap(*this);
    if (FAILED(pDefalutMap->Initialize(pArg)))
    {
        Safe_Release(pDefalutMap);
        MSG_BOX("CLONE FAIL : DEFAUTL MAP");
    }
    return pDefalutMap;
}

void CDefaultMap::Free()
{
    __super::Free();
}
