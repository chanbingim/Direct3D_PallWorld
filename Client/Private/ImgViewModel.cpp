#include "ImgViewModel.h"

#include "GameInstance.h"

CImgViewModel::CImgViewModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CImgViewModel::CImgViewModel(const CImgViewModel& rhs) :
    CPartObject(rhs)
{
}

HRESULT CImgViewModel::Initalize_Prototype()
{
    GAMEOBJECT_DESC Desc; 
    ZeroMemory(&Desc, sizeof(GAMEOBJECT_DESC));
    Desc.vScale = { 1.f, 1.f, 1.f };

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;


    return S_OK;
}

void CImgViewModel::Priority_Update(_float fDeletaTime)
{

}

void CImgViewModel::Update(_float fDeletaTime)
{
    if (nullptr == m_pVIBufferCom)
        return;

    if(m_IsAnimModel)
        m_pVIBufferCom->PlayAnimation(0, m_iAnimIndex, fDeletaTime);
}

void CImgViewModel::Late_Update(_float fDeletaTime)
{
    m_CombinedWorldMatrix = m_pTransformCom->GetWorldMat();
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CImgViewModel::Render()
{
    if (nullptr == m_pVIBufferCom)
        return E_FAIL;

    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    Bind_ShaderResources();

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        if (m_IsAnimModel)
            m_pShaderCom->Update_Shader(0);
        else
            m_NonAnimShader->Update_Shader(0);

        m_pVIBufferCom->Render(i);
    }

    return S_OK;
}

void CImgViewModel::Bind_Model(CModel* pModel)
{
    if (nullptr == pModel)
        m_pVIBufferCom = nullptr;
    else
    {
        if (m_pVIBufferCom)
            Safe_Release(m_pVIBufferCom);

        m_pVIBufferCom = static_cast<CModel*>(pModel->Clone(nullptr));
    }
   
}

void CImgViewModel::SetAnimationIndex(_uInt iIndex)
{
    m_iAnimIndex = iIndex;
}

HRESULT CImgViewModel::Bind_ShaderResources()
{
    if (m_IsAnimModel)
    {
        m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
        m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
        m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
        m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();
        m_pBoneMatrixEffect = m_pShaderCom->GetVariable("g_BoneMatrices")->AsMatrix();
    }
    else
    {
        m_pEMVWorldMat = m_NonAnimShader->GetVariable("g_WorldMatrix")->AsMatrix();
        m_pEMVViewMat = m_NonAnimShader->GetVariable("g_ViewMatrix")->AsMatrix();
        m_pEMVProjMat = m_NonAnimShader->GetVariable("g_ProjMatrix")->AsMatrix();
        m_pSRVEffect = m_NonAnimShader->GetVariable("g_DiffuseTexture")->AsShaderResource();
        m_pBoneMatrixEffect = m_NonAnimShader->GetVariable("g_BoneMatrices")->AsMatrix();
    }
  
   
    return S_OK;
}

HRESULT CImgViewModel::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedWorldMatrix));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    ID3D11ShaderResourceView* pResourceVeiw = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);

    if (m_IsAnimModel)
        m_pBoneMatrixEffect->SetMatrixArray(reinterpret_cast<const float*>(m_pVIBufferCom->GetBoneMatrices(iMeshIndex)), 0, m_pVIBufferCom->GetMeshNumBones(iMeshIndex));

    return S_OK;
}

HRESULT CImgViewModel::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("AnimShader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_NonAnimShader)))
        return E_FAIL;

    return S_OK;
}

CImgViewModel* CImgViewModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CImgViewModel* pModel = new CImgViewModel(pDevice, pContext);
    if (FAILED(pModel->Initalize_Prototype()))
    {
        Safe_Release(pModel);
        MSG_BOX("CREATE FAIL : IMG VIEW MODEL");
    }
    return pModel;
}

void CImgViewModel::Free()
{
    __super::Free();

    Safe_Release(m_NonAnimShader);
}
