#include "DefaultMap.h"

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

    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
        m_pNavigationCom[i]->Update(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    return S_OK;
}

void CDefaultMap::Priority_Update(_float fDeletaTime)
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for(_uInt i = 0; i < iNumMeshes; ++i)
        m_pNavigationCom[i]->Update(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

void CDefaultMap::Update(_float fDeletaTime)
{
    _float3 vOut = {};
#ifdef _DEBUG
  /*  if (m_bIsPicking)
    {
        if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 0) ||
            GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        {
            m_pVIBufferCom->UpdateHegiht(m_pTransformCom, 1.f, 2.f);
        }

        if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 1) ||
            GetAsyncKeyState(VK_RBUTTON) & 0x8000)
        {
            m_pVIBufferCom->UpdateHegiht(m_pTransformCom, -1.f, 2.f);
        }
    }*/
#endif // _DEBUG
}

void CDefaultMap::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CDefaultMap::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render(i);
        m_pNavigationCom[i]->Render({ 1.f, 0.f,0.f,1.f });
    }

    return S_OK;
}

_bool CDefaultMap::IsMoveTerrian(_vector vPosition)
{
    return _bool();
}

_Int CDefaultMap::FindCell(_vector vPosition)
{
    return _Int();
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

    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    m_pNavigationCom = new CNavigation*[iNumMeshes];

    WCHAR NaviComPath[MAX_PATH] = {};
    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        wsprintf(NaviComPath, TEXT("NaviMesh%d_Com"), i);
        m_pNavigationCom[i] = CNavigation::Create(m_pGraphic_Device, m_pDeviceContext, m_pVIBufferCom, i);
        m_pComponentMap.emplace(NaviComPath, m_pNavigationCom[i]);
    }

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
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
        Safe_Release(m_pNavigationCom[i]);

    Safe_Delete_Array(m_pNavigationCom);

    __super::Free();
}
